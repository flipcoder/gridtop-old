#include "CommandResolver.h"
#include "Motion.h"
#include "Operator.h"
#include "Action.h"
#include "WindowManager.h"
using namespace std;

CommandResolver :: CommandResolver(Commands* factory):
    m_pCommands(factory)
{
    assert(m_pCommands);

    // same order as in Commands.h
    m_pCommands->register_class<Motion>();
    m_pCommands->register_class<Operator>();
    //m_pCommands->.register_class<Action>();
    
    m_pCommands->register_resolver(bind(
        &CommandResolver::resolve,
        this,
        placeholders::_1
    ));
}

unsigned CommandResolver :: resolve(
    const std::tuple<
        WindowManager*,
        std::string
    >& args
) const {
    return std::numeric_limits<unsigned>::max();
}

