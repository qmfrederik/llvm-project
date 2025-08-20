//===-- GNUstepClassDescriptor.cpp ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GNUstepObjCClassDescriptor.h"

using namespace lldb;
using namespace lldb_private;

GNUstepClassDescriptor::GNUstepClassDescriptor(
    ObjCLanguageRuntime::ObjCISA isa, Process* process) {
  if (!isa || !process) {
    m_valid = false;
    return;
  }

  m_valid = true;

  Status error;

  ObjCLanguageRuntime::ObjCISA m_isa = process->ReadPointerFromMemory(isa, error);

  if (error.Fail()) {
    m_valid = false;
    return;
  }

  uint32_t ptr_size = process->GetAddressByteSize();

  if (!IsPointerValid(m_isa, ptr_size)) {
    m_valid = false;
    return;
  }

  m_parent_isa = process->ReadPointerFromMemory(m_isa + ptr_size, error);

  if (error.Fail()) {
    m_valid = false;
    return;
  }

  if (!IsPointerValid(m_parent_isa, ptr_size, true)) {
    m_valid = false;
    return;
  }

  lldb::addr_t name_ptr =
      process->ReadPointerFromMemory(m_isa + 2 * ptr_size, error);

  if (error.Fail()) {
    m_valid = false;
    return;
  }

  lldb::WritableDataBufferSP buffer_sp(new DataBufferHeap(1024, 0));

  size_t count = process->ReadCStringFromMemory(
      name_ptr, (char *)buffer_sp->GetBytes(), 1024, error);

  if (error.Fail()) {
    m_valid = false;
    return;
  }

  if (count)
    m_name = ConstString(reinterpret_cast<const char *>(buffer_sp->GetBytes()));
  else
    m_name = ConstString();

  m_instance_size = process->ReadUnsignedIntegerFromMemory(
      m_isa + 5 * ptr_size, ptr_size, 0, error);

  if (error.Fail()) {
    m_valid = false;
    return;
  }
}

GNUstepObjCRuntime::ClassDescriptorSP
GNUstepClassDescriptor::GetSuperclass() {
  return GNUstepObjCRuntime::ClassDescriptorSP();
}

GNUstepObjCRuntime::ClassDescriptorSP
GNUstepClassDescriptor::GetMetaclass() const {
  return GNUstepObjCRuntime::ClassDescriptorSP();
}