#include "CommandResolver.h"
#include "Motion.h"
#include "Operator.h"
#include "Action.h"
#include "Hint.h"
#include "WindowManager.h"
#include "Log.h"
#include "Util.h"

#include "FocusOperator.h"
#include "MoveOperator.h"

using namespace std;

CommandResolver :: CommandResolver(Commands* factory):
    m_pCommands(factory)
{
    assert(m_pCommands);

    unsigned i = 0;
    init_motions(i);
    init_operators(i);
    init_hints(i);
    init_actions(i);

    m_pCommands->register_resolver(bind(
        &CommandResolver::resolve,
        this,
        placeholders::_1
    ));
}

void CommandResolver :: init_motions(unsigned& offset)
{
    m_pCommands->register_class<Motion>();

    for(unsigned i=0;i<(unsigned)eMotion::MAX; ++i)
        m_CommandString[g_MotionString[i]] = offset; // same class

    ++offset; // same class
}

void CommandResolver :: init_operators(unsigned& offset)
{
    m_pCommands->register_class<FocusOperator>();
    m_pCommands->register_class<MoveOperator>();

    for(unsigned i=0;i<(unsigned)eOperator::MAX; ++i)
        m_CommandString[g_OperatorString[i]] = offset + i; // sub-classes

    offset += (unsigned)eOperator::MAX; // sub-classes
}

void CommandResolver :: init_hints(unsigned& offset)
{
    m_pCommands->register_class<Hint>();

    for(unsigned i=0;i<(unsigned)eHint::MAX; ++i)
        m_CommandString[g_HintString[i]] = offset; // same class

    ++offset; // same class
}

void CommandResolver :: init_actions(unsigned& offset)
{
    m_pCommands->register_class<Action>();

    for(unsigned i=0;i<(unsigned)eAction::MAX; ++i)
        m_CommandString[g_ActionString[i]] = offset; // same class

    ++offset; // same class
}

unsigned CommandResolver :: resolve(
    const std::tuple<
        WindowManager*,
        std::string
    >& args
) const {
    // trim prefix
    string cmd = std::get<1>(args);
    cmd = cmd.substr(
        cmd.find_first_not_of("+-")
    );

    try{
        return m_CommandString.at(cmd);
    }catch(const std::out_of_range&){
        return std::numeric_limits<unsigned>::max();
    }
}

