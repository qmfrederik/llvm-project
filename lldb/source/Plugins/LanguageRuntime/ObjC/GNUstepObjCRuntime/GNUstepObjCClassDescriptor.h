//===-- GNUstepObjCClassDescriptor.h ----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLDB_SOURCE_PLUGINS_LANGUAGERUNTIME_OBJC_GNUSTEPOBJCRUNTIME_GNUSTEPOBJCCLASSDESCRIPTOR_H
#define LLDB_SOURCE_PLUGINS_LANGUAGERUNTIME_OBJC_GNUSTEPOBJCRUNTIME_GNUSTEPOBJCCLASSDESCRIPTOR_H

#include <mutex>

#include "lldb/lldb-enumerations.h"
#include "lldb/lldb-private.h"

#include "Plugins/LanguageRuntime/ObjC/ObjCLanguageRuntime.h"
#include "Plugins/LanguageRuntime/ObjC/GNUstepObjCRuntime/GNUstepObjCRuntime.h"

namespace lldb_private {

class GNUstepClassDescriptor : public ObjCLanguageRuntime::ClassDescriptor {
  public: 
    GNUstepClassDescriptor(ObjCLanguageRuntime::ObjCISA isa, Process* process);
    ~GNUstepClassDescriptor() override = default;

    ConstString GetClassName() override { return m_name; }

    GNUstepObjCRuntime::ClassDescriptorSP GetSuperclass() override;

    GNUstepObjCRuntime::ClassDescriptorSP GetMetaclass() const override;

    bool IsValid() override { return m_valid; }

    // This implementation does not support tagged pointers
    bool GetTaggedPointerInfo(uint64_t *info_bits = nullptr,
                              uint64_t *value_bits = nullptr,
                              uint64_t *payload = nullptr) override {
      return false;
    }

    bool GetTaggedPointerInfoSigned(uint64_t *info_bits = nullptr,
                                    int64_t *value_bits = nullptr,
                                    uint64_t *payload = nullptr) override {
      return false;
    }

    uint64_t GetInstanceSize() override { return m_instance_size; }

    ObjCLanguageRuntime::ObjCISA GetISA() override { return m_isa; }

  private:
    ObjCLanguageRuntime::ObjCISA m_isa;
    ObjCLanguageRuntime::ObjCISA m_parent_isa;
    ConstString m_name;
    uint64_t m_instance_size;
    bool m_valid;
};

} // namespace lldb_private

#endif // LLDB_SOURCE_PLUGINS_LANGUAGERUNTIME_OBJC_GNUSTEPOBJCRUNTIME_GNUSTEPOBJCCLASSDESCRIPTOR_H
