#include "BufferedLogger.h"

BufferedLogger::BufferedLogger()
{
    head = 0;
    tail = 0;
    count = 0;
    droppedRecords = 0;
    totalBuffered = 0;
}

bool BufferedLogger::push(
    const TelemetryRecord& record)
{
    if (isFull())
    {
        droppedRecords++;
        return false;
    }

    buffer[head] = record;

    head =
        (head + 1) %
        BUFFERED_LOGGER_CAPACITY;

    count++;
    totalBuffered++;

    return true;
}

bool BufferedLogger::peek(
    TelemetryRecord& record) const
{
    if (isEmpty())
    {
        return false;
    }

    record = buffer[tail];

    return true;
}

bool BufferedLogger::pop(
    TelemetryRecord& record)
{
    if (isEmpty())
    {
        return false;
    }

    record = buffer[tail];

    tail =
        (tail + 1) %
        BUFFERED_LOGGER_CAPACITY;

    count--;

    return true;
}

void BufferedLogger::clear()
{
    head = 0;
    tail = 0;
    count = 0;
}

bool BufferedLogger::isEmpty() const
{
    return count == 0;
}

bool BufferedLogger::isFull() const
{
    return count >= BUFFERED_LOGGER_CAPACITY;
}

size_t BufferedLogger::size() const
{
    return count;
}

size_t BufferedLogger::capacity() const
{
    return BUFFERED_LOGGER_CAPACITY;
}

size_t BufferedLogger::freeSpace() const
{
    return BUFFERED_LOGGER_CAPACITY - count;
}

uint32_t BufferedLogger::getDroppedRecords() const
{
    return droppedRecords;
}

uint32_t BufferedLogger::getTotalBuffered() const
{
    return totalBuffered;
}