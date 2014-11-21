/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WorkerPermissionClient_h
#define WorkerPermissionClient_h

#include "core/workers/WorkerClients.h"
#include "wtf/Forward.h"

namespace WebCore {
class ExecutionContext;
}

namespace blink {

class WebFrame;
class WebPermissionCallbacks;
class WebString;
class WebWorkerPermissionClientProxy;

class WorkerPermissionClient FINAL : public NoBaseWillBeGarbageCollectedFinalized<WorkerPermissionClient>, public WillBeHeapSupplement<WebCore::WorkerClients> {
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(WorkerPermissionClient);
public:
    static PassOwnPtrWillBeRawPtr<WorkerPermissionClient> create(PassOwnPtr<WebWorkerPermissionClientProxy>);

    virtual ~WorkerPermissionClient();

    bool allowDatabase(const WebString& name, const WebString& displayName, unsigned long estimatedSize);
    bool allowFileSystem();
    void requestFileSystemAccess(const WebPermissionCallbacks&);
    bool allowIndexedDB(const WebString& name);

    static const char* supplementName();
    static WorkerPermissionClient* from(WebCore::ExecutionContext&);

    virtual void trace(WebCore::Visitor* visitor) OVERRIDE { WillBeHeapSupplement<WebCore::WorkerClients>::trace(visitor); }

private:
    explicit WorkerPermissionClient(PassOwnPtr<WebWorkerPermissionClientProxy>);

    OwnPtr<WebWorkerPermissionClientProxy> m_proxy;
};

void providePermissionClientToWorker(WebCore::WorkerClients*, PassOwnPtr<WebWorkerPermissionClientProxy>);

} // namespace blink

#endif // WorkerPermissionClient_h
