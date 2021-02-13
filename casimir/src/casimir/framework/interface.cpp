#include "interface.hpp"

#include <utility>
#include "../core/private-context.hpp"

namespace Casimir::framework {
    
    CASIMIR_EXPORT AbstractInterface::AbstractInterface(CasimirContext ctx, utilities::Uuid uuid)
    : ContextualObject(ctx), IndexableObject(std::move(uuid)) {
        ctx->logger(PrivateLogging::Note) << "Register interface " << uuid;
        ctx->interfaces.insert(this);
        ctx->interfacesByUuid.insert(std::make_pair(uuid, this));
    }
    
    CASIMIR_EXPORT AbstractInterface::~AbstractInterface() {
        ctx()->logger(PrivateLogging::Note) << "Unregister interface " << uuid();
        auto it = ctx()->interfaces.find(this);
        ctx()->interfaces.erase(it);
        auto cit = ctx()->interfacesByUuid.find(uuid());
        ctx()->interfacesByUuid.erase(cit);
    }
    
};