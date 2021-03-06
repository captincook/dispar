#include "Section.h"

Section::Section(Section::Type type, const QString &name, quint64 addr, quint64 size,
                 quint32 offset)
  : type_{type}, name_{name}, addr{addr}, size_{size}, offset_{offset}, disasm(nullptr)
{
}

QString Section::typeName(Type type)
{
  switch (type) {
  case Section::Type::TEXT:
    return "Text";

  case Section::Type::SYMBOL_STUBS:
    return "Symbol Stubs";

  case Section::Type::SYMBOLS:
    return "Symbols";

  case Section::Type::DYN_SYMBOLS:
    return "Dynamic Symbols";

  case Section::Type::CSTRING:
    return "CString";

  case Section::Type::STRING:
    return "String";

  case Section::Type::FUNC_STARTS:
    return "Function Starts";

  case Section::Type::CODE_SIG:
    return "Code Signatures";
  }

  return "";
}

Section::Type Section::type() const
{
  return type_;
}

QString Section::name() const
{
  return name_;
}

quint64 Section::address() const
{
  return addr;
}

quint64 Section::size() const
{
  return size_;
}

quint32 Section::offset() const
{
  return offset_;
}

const QByteArray &Section::data() const
{
  return data_;
}

void Section::setData(const QByteArray &data)
{
  data_ = data;
}

void Section::setSubData(const QByteArray &subData, int pos)
{
  if (pos < 0 || pos > data_.size() - 1) {
    return;
  }

  data_.replace(pos, subData.size(), subData);
  modified = QDateTime::currentDateTime();

  QPair<int, int> region(pos, subData.size());
  if (!modifiedRegions_.contains(region)) {
    modifiedRegions_ << region;
  }
}

bool Section::isModified() const
{
  return !modifiedRegions_.isEmpty();
}

QDateTime Section::modifiedWhen() const
{
  return modified;
}

const QList<QPair<int, int>> &Section::modifiedRegions() const
{
  return modifiedRegions_;
}

void Section::setDisassembly(std::shared_ptr<Disassembler::Result> disasm)
{
  this->disasm = disasm;
}

std::shared_ptr<Disassembler::Result> Section::disassembly() const
{
  return disasm;
}
