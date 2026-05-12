//
// Created by Nico Russo on 5/12/26.
//

#ifndef SANDBOX_ENTRYPOINT_H
#define SANDBOX_ENTRYPOINT_H

#include "application.h"

/* ================================================================================================================ */
/* ENTRY POINT ==================================================================================================== */
/* ================================================================================================================ */

extern Application* createApplication();

int main(int argc, char** argv) {
    LOG_INFO("Program running");

    const auto app = std::unique_ptr<Application>(createApplication());
    app->run();
}

#endif //SANDBOX_ENTRYPOINT_H
