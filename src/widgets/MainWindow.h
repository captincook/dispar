#ifndef DISPAR_MAIN_WINDOW_H
#define DISPAR_MAIN_WINDOW_H

#include <QList>
#include <QMainWindow>

#include <memory>

#include "../CpuType.h"

class QAction;

class Format;
class FormatLoader;
class BinaryWidget;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(const QString &file = QString());
  virtual ~MainWindow();

protected:
  void showEvent(QShowEvent *event);
  void closeEvent(QCloseEvent *event);

private slots:
  void newProject();
  void openProject(const QString &projectFile = QString());
  void saveProject();
  void closeProject();
  void openBinary();

  void onRecentProject();
  void onRecentBinary();
  void onConversionHelper();
  void onDisassembler();
  void onAbout();
  void onOptions();
  void onLoadSuccess(std::shared_ptr<Format> fmt);
  void onProjectModified();

private:
  void setTitle(const QString &file = QString());
  void readSettings();
  void createLayout();
  void createMenu();
  void loadBinary(QString file);

  /// If modified ask to save.
  /** Returns false if cancelled. */
  bool checkSave();

  bool modified;
  QString startupFile;
  QStringList recentProjects, recentBinaries;
  QByteArray geometry;

  QAction *newProjectAction, *saveProjectAction, *saveAsProjectAction, *closeProjectAction;

  std::unique_ptr<FormatLoader> loader;
};

#endif // DISPAR_MAIN_WINDOW_H
