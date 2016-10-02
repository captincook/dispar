#include "TagsEdit.h"
#include "../Context.h"
#include "../Project.h"

#include <QDebug>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QVBoxLayout>

TagsEdit::TagsEdit() : address(0)
{
  createLayout();
}

void TagsEdit::setAddress(quint64 address)
{
  this->address = address;

  auto project = Context::get().project();
  listWidget->clear();
  listWidget->addItems(project->addressTags(address));
  listWidget->setEnabled(true);

  lineEdit->clear();
  lineEdit->setEnabled(true);
}

void TagsEdit::onReturnPressed()
{
  auto tag = lineEdit->text().simplified();
  if (tag.isEmpty()) return;

  auto project = Context::get().project();
  if (!project->addAddressTag(tag, address)) {
    QMessageBox::warning(this, "dispar", tr("Tag is already used!"));
    return;
  }

  lineEdit->clear();
  listWidget->addItem(tag);
}

void TagsEdit::createLayout()
{
  listWidget = new QListWidget;
  listWidget->setEnabled(false);
  // TODO: icon mode = flow layout
  // TODO: delegate to draw as "blue bubbles"

  lineEdit = new QLineEdit;
  lineEdit->setEnabled(false);
  connect(lineEdit, &QLineEdit::returnPressed, this, &TagsEdit::onReturnPressed);

  auto *layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(listWidget);
  layout->addWidget(lineEdit);

  setLayout(layout);
}
