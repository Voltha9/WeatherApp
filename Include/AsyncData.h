#pragma once

#include <pplx/pplxtasks.h>
#include <optional>

template<typename DataType>
struct AsyncData
{
    std::optional<DataType> data;
    Concurrency::task<std::optional<DataType>> task;

    void ResetTask(Concurrency::task<std::optional<DataType>> newTask)
    {
        task = newTask;
        data.reset();
    }

    bool IsTaskCompleted()
    {
        return task.is_done() && !data;
    }

    void RetrieveData()
    {
        if (IsTaskCompleted())
        {
            data = task.get();
        }
    }
};