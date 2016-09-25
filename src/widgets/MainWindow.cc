#include "MainWindow.h"
#include "../BinaryObject.h"
#include "../Disassembler.h"
#include "../Util.h"
#include "../Version.h"
#include "../formats/Format.h"
#include "AboutDialog.h"
#include "BinaryWidget.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSet>
#include <QSettings>
#include <QVBoxLayout>

MainWindow::MainWindow(const QString &file)
  : shown(false), modified(false), startupFile(file), binaryWidget(nullptr)
{
  setTitle();
  readSettings();
  createLayout();
  createMenu();
}

MainWindow::~MainWindow()
{
  QSettings settings;
  settings.setValue("MainWindow.geometry", saveGeometry());
  settings.setValue("MainWindow.recentFiles", recentFiles);
}

void MainWindow::showEvent(QShowEvent *event)
{
  QMainWindow::showEvent(event);

  if (shown) return;
  shown = true;

  if (geometry.isEmpty()) {
    resize(900, 500);
    Util::centerWidget(this);
  }
  else {
    restoreGeometry(geometry);
  }

  // Load specified files or open file dialog.
  if (startupFile.isEmpty()) {
    openBinary();
  }
  else {
    loadBinary(startupFile);
  }
}

void MainWindow::openBinary()
{
  QFileDialog diag(this, tr("Open Binary"), QDir::homePath());
  diag.setNameFilters(
    QStringList{"Mach-O binary (*.o *.dylib *.dylinker *.bundle *.app *)", "Any file (*)"});

  if (!diag.exec()) {
    if (!binaryWidget) {
      qApp->quit();
    }
    return;
  }

  auto file = diag.selectedFiles().first();
  loadBinary(file);
}

void MainWindow::onRecentFile()
{
  auto *action = qobject_cast<QAction *>(sender());
  if (!action) return;
  loadBinary(action->text());
}

void MainWindow::onAbout()
{
  AboutDialog diag;
  diag.exec();
}

void MainWindow::setTitle(const QString &file)
{
  setWindowTitle(QString("Dispar v%1%2")
                   .arg(versionString())
                   .arg(!file.isEmpty() ? QString(" - %1").arg(file) : ""));
}

void MainWindow::readSettings()
{
  QSettings settings;
  geometry = settings.value("MainWindow.geometry").toByteArray();

  // Load recent files.
  recentFiles = settings.value("MainWindow.recentFiles", QStringList()).toStringList();
  for (int i = recentFiles.size() - 1; i >= 0; i--) {
    if (!QFile::exists(recentFiles[i])) {
      recentFiles.removeAt(i);
    }
  }
  if (recentFiles.size() > 10) {
    recentFiles = recentFiles.mid(recentFiles.size() - 10);
  }
}

void MainWindow::createLayout()
{
  // Doing nothing right now.
}

void MainWindow::createMenu()
{
  auto *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(tr("Open binary"), this, SLOT(openBinary()), QKeySequence::Open);

  if (!recentFiles.isEmpty()) {
    auto *recentMenu = fileMenu->addMenu(tr("Open recent files"));
    for (const auto &file : recentFiles) {
      recentMenu->addAction(file, this, SLOT(onRecentFile()));
    }
  }

  auto *helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(tr("&About"), this, SLOT(onAbout()));
}

void MainWindow::loadBinary(QString file)
{
  // Resolve absolute path.
  file = QFileInfo(file).absoluteFilePath();

  // If .app then resolve the internal binary file.
  QString appBin = Util::resolveAppBinary(file);
  if (!appBin.isEmpty()) {
    file = appBin;
  }

  qDebug() << "Loading binary:" << file;

  QProgressDialog progDiag(this);
  progDiag.setLabelText(tr("Detecting format.."));
  progDiag.setCancelButton(nullptr);
  progDiag.setRange(0, 0);
  progDiag.show();
  qDebug() << qPrintable(progDiag.labelText());
  qApp->processEvents();

  auto fmt = Format::detect(file);
  if (fmt == nullptr) {
    QMessageBox::critical(this, "dispar", tr("Unknown file - could not open!"));
    return;
  }

  qDebug() << "detected:" << Format::typeName(fmt->type());

  progDiag.setLabelText(tr("Reading and parsing binary.."));
  qDebug() << qPrintable(progDiag.labelText());
  qApp->processEvents();
  if (!fmt->parse()) {
    QMessageBox::warning(this, "dispar", tr("Could not parse file!"));
    return;
  }

  // Update window title with path.
  setTitle(file);

  // Disassemble code sections of all binary objects.
  progDiag.setLabelText(tr("Disassembling code sections.."));
  qDebug() << qPrintable(progDiag.labelText());
  qApp->processEvents();
  for (auto &object : fmt->objects()) {
    Disassembler dis(object);
    if (dis.valid()) {
      for (auto &sec : object->sections()) {
        switch (sec->type()) {
        case Section::Type::TEXT:
        case Section::Type::SYMBOL_STUBS: {
          auto res = dis.disassemble(sec->data());
          if (res) {
            sec->setDisassembly(res);
          }
          break;
        }

        default:
          break;
        }
      }
    }
  }

  // Add recent file.
  if (!recentFiles.contains(file)) {
    recentFiles << file;
  }
  if (recentFiles.size() > 10) {
    recentFiles.removeFirst();
  }

  if (centralWidget()) {
    centralWidget()->deleteLater();
  }
  binaryWidget = new BinaryWidget(fmt);
  setCentralWidget(binaryWidget);
  // connect(binWidget, &BinaryWidget::modified, this, &MainWindow::onBinaryObjectModified);
}
