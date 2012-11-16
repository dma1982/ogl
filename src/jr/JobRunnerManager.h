#ifndef __OGL_JOB_MANAGER_H__
#define __OGL_JOB_MANAGER_H__

#include <ace/Singleton.h>
#include <ace/Task.h>

#include <map>

#include "Commands.h"
#include "ogl.h"

namespace ogl
{
    /**
     * Job manager
     */
    class JobRunnerManager : public ACE_Task<ACE_MT_SYNCH>
    {
        public:
            JobRunnerManager();
            ~JobRunnerManager();

            virtual int open();

            virtual int svc();

            virtual int close(unsigned long);

            int sendCommand(ogl::Command* cmd);

            void shutdown(void);

        private:
            Command* nextCommand();
            bool m_shutdown;
    };

    typedef ACE_Singleton<JobRunnerManager, ACE_Null_Mutex> JOBRUNNERMGR;
};


#endif

