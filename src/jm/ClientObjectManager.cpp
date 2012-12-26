#include "ClientObjectManager.h"

#include "JobManager.h"

namespace ogl
{

    ClientHandlerObject::ClientHandlerObject()
    {
        m_jobManager = JOBMANAGER::instance();
    }

    int ClientHandlerObject::CreateJob(ogl::CommandHeader& header, ogl::JobOption& jobOption)
    {

        ogl::CommandHeader completeHeader(CreateJobComplete, header.contextId());
        ogl::CommandHeader failedHeader(CreateJobFailed, header.contextId());

        int hr = m_jobManager->addJob(jobOption);

        if (hr < 0)
        {
            return HandlerObject::sendResponse(failedHeader);
        }

        return HandlerObject::sendResponse(completeHeader, &jobOption);
    }

    int ClientHandlerObject::CreateTask(ogl::CommandHeader& header, ogl::TaskOption& taskOption)
    {

        ogl::CommandHeader completeHeader(CreateTaskComplete, header.contextId());
        ogl::CommandHeader failedHeader(CreateTaskFailed, header.contextId());

        Job* job = m_jobManager->getJob(taskOption.jobId());

        if (job == 0)
        {
            return HandlerObject::sendResponse(failedHeader);
        }

        Task* task = job->addTask(taskOption);

        if (task == 0)
        {
            return HandlerObject::sendResponse(failedHeader);
        }

        return HandlerObject::sendResponse(completeHeader, task->taskOption());
    }

    int ClientHandlerObject::FetchTaskOutput(ogl::CommandHeader& header, ogl::TaskOption& taskOption)
    {
        ogl::CommandHeader completeHeader(FetchTaskOutputComplete, header.contextId());
        ogl::CommandHeader failedHeader(FetchTaskOutputFailed, header.contextId());

        Job* job = m_jobManager->getJob(taskOption.jobId());

        if (job == 0)
        {
            return HandlerObject::sendResponse(failedHeader);
        }

        Task* task = job->getTask(taskOption.taskId());
        if (task == 0)
        {
            return HandlerObject::sendResponse(failedHeader);
        }

        if (task->isCompleted())
        {
            return HandlerObject::sendResponse(completeHeader, task->taskOption());
        }

        task->addObserver(header.contextId(), this);
        return 0;
    }

    int ClientHandlerObject::CloseJob(ogl::CommandHeader& header, ogl::JobOption& jobOption)
    {
        ogl::CommandHeader completeHeader(CloseJobComplete, header.contextId());
        ogl::CommandHeader failedHeader(CloseJobFailed, header.contextId());

        Job* job = m_jobManager->getJob(jobOption.id());

        if (job == 0)
        {
            return HandlerObject::sendResponse(failedHeader);
        }

        int hr = job->closeJob();

        if (hr < 0)
        {
            return HandlerObject::sendResponse(failedHeader);
        }

        return HandlerObject::sendResponse(completeHeader, &jobOption);
    }

    /*
     * Command router
     */
    int ClientHandlerObject::executeRequest(ogl::CommandHeader& cmd, ACE_Message_Block& data)
    {
        switch (cmd.commandType())
        {
        case CreateJobCommand:
        {
            ogl::JobOption jobOption;
            jobOption.deserialize(&data);
            CreateJob(cmd, jobOption);
            break;
        }
        case CreateTaskCommand:
        {
            ogl::TaskOption taskOption;
            taskOption.deserialize(&data);
            CreateTask(cmd, taskOption);
            break;
        }
        case CloseJobCommand:
        {
            ogl::JobOption jobOption;
            jobOption.deserialize(&data);
            CloseJob(cmd, jobOption);
            break;
        }
        case FetchTaskOutputCommand:
        {
            ogl::TaskOption taskOption;
            taskOption.deserialize(&data);
            FetchTaskOutput(cmd, taskOption);
            break;
        }
        case Unknown:
        default:
            return 0;
        }
        return 0;
    }
}
