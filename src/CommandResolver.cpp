#include "CommandResolver.h"
#include "Motion.h"
#include "Operator.h"
#include "Action.h"
#include "WindowManager.h"
#include "Log.h"
#include "Util.h"

#include "FocusOperator.h"

using namespace std;

CommandResolver :: CommandResolver(Commands* factory):
    m_pCommands(factory)
{
    assert(m_pCommands);

    init_motions();
    init_operators();

    m_pCommands->register_resolver(bind(
        &CommandResolver::resolve,
        this,
        placeholders::_1
    ));
}

void CommandResolver :: init_motions()
{
    m_pCommands->register_class<Motion>();

    for(unsigned i=0;i<(unsigned)eMotion::MAX; ++i)
        m_CommandString[g_MotionString[i]] = (unsigned)eCommand::MOTION;
}

void CommandResolver :: init_operators()
{
    m_pCommands->register_class<FocusOperator>();

    for(unsigned i=0;i<(unsigned)eOperator::MAX; ++i)
        m_CommandString[g_OperatorString[i]] =
            (unsigned)eCommand::OPERATOR + i;
}

unsigned CommandResolver :: resolve(
    const std::tuple<
        WindowManager*,
        std::string
    >& args
) const {
    try{
        LOGf("Command: %s", std::get<1>(args));
        return m_CommandString.at(std::get<1>(args));
    }catch(const std::out_of_range&){
        return std::numeric_limits<unsigned>::max();
    }
}

