/*
* Copyright (c) 2014 - 2015, The Linux Foundation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
*    * Redistributions of source code must retain the above copyright notice, this list of
*      conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above copyright notice, this list of
*      conditions and the following disclaimer in the documentation and/or other materials provided
*      with the distribution.
*    * Neither the name of The Linux Foundation nor the names of its contributors may be used to
*      endorse or promote products derived from this software without specific prior written
*      permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <utils/constants.h>
#include <utils/debug.h>

#include "display_virtual.h"
#include "hw_virtual_interface.h"
#include "hw_info_interface.h"

#define __CLASS__ "DisplayVirtual"

namespace sde {

DisplayVirtual::DisplayVirtual(DisplayEventHandler *event_handler, HWInfoInterface *hw_info_intf,
                               BufferSyncHandler *buffer_sync_handler, CompManager *comp_manager,
                               OfflineCtrl *offline_ctrl)
  : DisplayBase(kVirtual, event_handler, kDeviceVirtual, buffer_sync_handler, comp_manager,
                offline_ctrl), hw_info_intf_(hw_info_intf) {
}

DisplayError DisplayVirtual::Init() {
  SCOPE_LOCK(locker_);

  DisplayError error = HWVirtualInterface::Create(&hw_virtual_intf_, hw_info_intf_,
                                                  DisplayBase::buffer_sync_handler_);
  if (error != kErrorNone) {
    return error;
  }
  DisplayBase::hw_intf_ = hw_virtual_intf_;

  error = DisplayBase::Init();
  if (error != kErrorNone) {
    HWVirtualInterface::Destroy(hw_virtual_intf_);
  }

  return error;
}

DisplayError DisplayVirtual::Deinit() {
  SCOPE_LOCK(locker_);

  DisplayError error = DisplayBase::Deinit();
  if (error != kErrorNone) {
    return error;
  }
  HWVirtualInterface::Destroy(hw_virtual_intf_);

  return error;
}

}  // namespace sde
