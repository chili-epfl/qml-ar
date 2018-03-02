#include "mvpprovider.h"
#include "timelogger.h"

void MVPProvider::setMVPMatrix(QMatrix4x4 mat)
{
    if(mat != mvp_matrix)
    {
        if(!isValid(mvp_matrix))
            TimeLoggerLog("%s", "Matrix available");

        mvp_matrix = mat;

        // obtaining new info
        PipelineContainerInfo info = object_in_process.checkpointed(typeid(*this).name());

        emit newPMatrix(PipelineContainer<QMatrix4x4>(p_matrix, info));
        emit newMVMatrix(PipelineContainer<QMatrix4x4>(mv_matrix, info));
        emit newMVPMatrix(PipelineContainer<QMatrix4x4>(mvp_matrix, info));
        emit newInfo(info);
    }
}

MVPProvider::MVPProvider()
{
    reset();
}

void MVPProvider::reset()
{
    if(isValid(mvp_matrix))
    {
        TimeLoggerLog("%s", "Hiding objects");
        mvp_matrix.setToIdentity();
        mv_matrix.setToIdentity();

        // obtaining new info
        PipelineContainerInfo info = object_in_process.checkpointed(typeid(*this).name());

        emit newMVPMatrix(PipelineContainer<QMatrix4x4>(mvp_matrix, info));
        emit newInfo(info);
    }
}

bool MVPProvider::isValid(QMatrix4x4 mat)
{
    return !mat.isIdentity();
}
