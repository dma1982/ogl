#ifndef __OGL_CONF_H__
#define __OGL_CONF_H__

#include "types.h"
#include "event.h"
#include <cstdio>

#include <string>

#include <libconfig.h++>

#include <log4cxx/logger.h>

#define OGL_MASTER_HOST "ogl.master"
#define OGL_MASTER_JR_PORT "ogl.port.jr"
#define OGL_MASTER_CLI_PORT "ogl.port.cli"

#define OGL_SCHEDULER_INTERVAL "ogl.scheduler.interval"
#define OGL_SCHEDULER_POLICY "ogl.scheduler.policy"

#define OGL_JR_ID "jr.id"
#define OGL_JR_CORES "jr.cores"

#define OGL_LOG_FILE "log.file"
#define OGL_LOG_LEVEL "log.level"
#define OGL_MEM_POOL "mem.pool"


#define OGL_DEFAULT_LOG_FILE "ogl.log"

namespace ogl
{
    class Configuration
    {
        private:
            Configuration();

            static Configuration* m_instance;
            static lock_t m_lock;

            FILE* m_logFile;

            libconfig::Config* m_conf;

            LOG_LEVEL m_logLevel;

            MEM_POOL_TYPE m_memPoolType;

            int m_masterJrPort;
            int m_masterCliPort;

            std::string m_masterHost;

            std::string m_runnerId;

            size_t m_runnerCores;

            unsigned int m_schedulerInterval;

        public:

            static Configuration* instance();

            ~Configuration();

            int read(const char* path);

            FILE* getLogFile();

            int getMasterCliPort();

            int getMasterJrPort();

            const std::string& getRunnerId();

            size_t getRunnerCores();

            unsigned int getSchedulerInterval();

            const std::string& getMasterHost();

            LOG_LEVEL getLogLevel();

            log4cxx::Logger* getLogger(const char* name);

            MEM_POOL_TYPE getMemPoolType();
    };

}

#define OGLCONF (ogl::Configuration::instance())

#endif
