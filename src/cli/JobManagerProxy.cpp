#include "ogl.h"
#include "JobManagerProxy.h"
#include "Object.h"
#include "Commands.h"
#include "JobProxy.h"

#include "Exception.h"

#include <algorithm>

namespace ogl
{

    class JobManagerClient : public Client <JobManagerProxy>
    {
    };

    JobManagerProxy::~JobManagerProxy()
    {
        // release JobProxys
        std::for_each(m_jobProxyList.begin(), m_jobProxyList.end(), releaseObject<JobProxy>);
    }

    JobManagerProxy::JobManagerProxy()
    {
    }

    JobManagerProxy* JobManagerProxy::createInstance()
    {
        JobManagerClient* client;
        ACE_NEW_RETURN(client, JobManagerClient(), 0);

        client->start(ogl::Configuration::instance()->getMasterHost(),
                      ogl::Configuration::instance()->getMasterCliPort());

        return client->get_handler();
    }

    int JobManagerProxy::executeRequest(ogl::CommandHeader& header, ACE_Message_Block& data)
    {
        return ClientActionManager::signalAction(header, data.duplicate());
    }

    JobProxy* JobManagerProxy::addJob(JobOption* jobOption)
    {
        ClientAction action(this);

        action.submit(ogl::CreateJobCommand, jobOption);

        action.wait();

        if (action.returnCode() == ogl::CreateJobFailed)
        {
            OGL_THROW_EXCEPTION("Failed to receive response from Job Manager Server.");
        }

        return new JobProxy(action.getResponse(), this);
    }
};