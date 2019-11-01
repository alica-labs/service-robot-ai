#pragma once

#include <srgsim/SRGEnums.h>
#include <srgsim/containers/Coordinate.h>
#include <essentials/IdentifierConstPtr.h>
namespace srg
{
class SRGWorldModel;
namespace dialogue
{
class Task
{
public:
    Task();
    virtual ~Task() = default;

    virtual bool checkSuccess(SRGWorldModel* wm) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const srg::dialogue::Task& obj)
    {
        os << "Task: Type: " << static_cast<int>(obj.type) << " at " << obj.coordinate;
        return os;
    }

    essentials::IdentifierConstPtr senderID;
    essentials::IdentifierConstPtr receiverID;

    srgsim::TaskType type;
    srgsim::Coordinate coordinate;

    essentials::IdentifierConstPtr actID;
    essentials::IdentifierConstPtr previousActID;
};
} // namespace dialogue
} // namespace srg
