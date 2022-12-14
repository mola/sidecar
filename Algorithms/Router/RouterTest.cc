#include "ace/FILE_Connector.h"
#include "ace/Reactor.h"
#include "ace/Stream.h"

#include "Algorithms/Controller.h"
#include "Algorithms/ShutdownMonitor.h"
#include "IO/FileWriterTask.h"
#include "IO/MessageManager.h"
#include "IO/Module.h"
#include "IO/ProcessingStateChangeRequest.h"
#include "IO/Readers.h"
#include "IO/ShutdownRequest.h"

#include "Logger/Log.h"
#include "Messages/Video.h"
#include "Parameter/Parameter.h"
#include "UnitTest/UnitTest.h"
#include "Utils/FilePath.h"

#include "Router.h"

using namespace SideCar::Algorithms;
using namespace SideCar::IO;
using namespace SideCar::Messages;

struct Test : public UnitTest::TestObj {
    Test() : UnitTest::TestObj("Router") {}
    void test();
};

void
Test::test()
{
    Logger::Log::Root().setPriorityLimit(Logger::Priority::kDebug);
}

int
main(int argc, const char* argv[])
{
    return Test().mainRun();
}
