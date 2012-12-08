#include "ogl.h"
#include "JobManager.h"
#include "ClientManager.h"
#include "JobRunnerManager.h"

#include <iostream>

int main(int argc, char** argv)
{
    ACE::init();

    // start job manager
    try
    {
        ogl::CLIMGR::instance()->start(ogl::Configuration::instance()->getMasterCliPort());

        ogl::JRMGR::instance()->start(ogl::Configuration::instance()->getMasterJrPort());

        OGL_LOG_INFO("Job Manager Server start at <%d>.", ogl::Configuration::instance()->getMasterCliPort());
        OGL_LOG_INFO("JobRunner Manager Server start at <%d>.", ogl::Configuration::instance()->getMasterJrPort());

        if (ACE_Thread_Manager::instance()->wait() < 0)
        {
            OGL_LOG_ERROR("Failed to wait all thread.");
        }

        OGL_LOG_INFO("Job Manager Daemon stop.");
    }
    catch (ogl::Exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    ACE::fini();

    return 0;
}
