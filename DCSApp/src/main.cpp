/*
 * Copyright (c) 2017 Baidu, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <signal.h>
#include <cstdlib>
#include <string>
#include <execinfo.h>
#include "LoggerUtils/DcsSdkLogger.h"
#include "DCSApp/DCSApplication.h"
#include "DCSApp/DeviceIoWrapper.h"
#include <DeviceTools/SingleApplication.h>
#include <DeviceTools/PrintTickCount.h>

#ifndef __SAMPLEAPP_VERSION__
#define __SAMPLEAPP_VERSION__ "Unknown SampleApp Version"
#endif

#ifndef __DCSSDK_VERSION__
#define __DCSSDK_VERSION__ "Unknown DcsSdk Version"
#endif

#ifndef __DUER_LINK_VERSION__
#define __DUER_LINK_VERSION__ "Unknown DuerLink Version"
#endif

static void (*segmentation_default_handler)(int);
static void segmentation_signal_handler(int arg)
{
    int nptrs = 0;
#define BACK_TRACE_SIZE 100
    void *buffer[BACK_TRACE_SIZE];
    int core_dump_fd = 0;

    signal(SIGSEGV, segmentation_default_handler);

    core_dump_fd = open("/data/core_dump_stack", O_CREAT | O_APPEND | O_RDWR);
    if (core_dump_fd > 0) {
        nptrs = backtrace(buffer, BACK_TRACE_SIZE);
        backtrace_symbols_fd(buffer, nptrs, core_dump_fd);
        write(core_dump_fd, "\n", sizeof("\n"));
        fsync(core_dump_fd);
        close(core_dump_fd);
    }

    kill(getpid(), SIGSEGV);
}

int main(int argc, char** argv) {
    deviceCommonLib::deviceTools::printTickCount("duer_linux main begin");
#ifdef Build_CrabSdk
    /**
     * 首先用Crab平台分配的App Key和宿主程序的版本进行初始化
     * 最好在main函数中进行调用
     */
    std::string m_softwareVersion = duerOSDcsApp::application::DeviceIoWrapper::getInstance()->getVersion();
    std::string m_deviceId = duerOSDcsApp::application::DeviceIoWrapper::getInstance()->getDeviceId();
    baidu_crab_sdk::CrabSDK::init("78ceb6f2024be4e7", m_softwareVersion, "/tmp/coredump");
    /**
     * 设置设备DEVICEID
     */
    baidu_crab_sdk::CrabSDK::set_did(m_deviceId);
#endif
    segmentation_default_handler = signal(SIGSEGV, segmentation_signal_handler);
#ifdef MTK8516
    if (geteuid() != 0) {
        APP_ERROR("This program must run as root, such as \"sudo ./duer_linux\"");
        return 1;
    }
    if (deviceCommonLib::deviceTools::SingleApplication::is_running()) {
        APP_ERROR("duer_linux is already running");
        return 1;
    }
#endif

    /// print current version
    APP_INFO("SampleApp Version: [%s]", __SAMPLEAPP_VERSION__);
    APP_INFO("DcsSdk Version: [%s]", __DCSSDK_VERSION__);
    APP_INFO("DuerLink Version: [%s]", __DUER_LINK_VERSION__);

    auto dcsApplication = duerOSDcsApp::application::DCSApplication::create();

    if (!dcsApplication) {
        APP_ERROR("Failed to create to SampleApplication!");
        duerOSDcsApp::application::SoundController::getInstance()->release();
        duerOSDcsApp::application::DeviceIoWrapper::getInstance()->release();
        return EXIT_FAILURE;
    }

    // This will run until application quit.
    dcsApplication->run();

    return EXIT_SUCCESS;
}
