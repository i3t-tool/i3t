import{_ as e,o as t,c as s,Q as n}from"./chunks/framework.0f749972.js";const g=JSON.parse('{"title":"Workspace::CoreOutPin","description":"","frontmatter":{"title":"Workspace::CoreOutPin"},"headers":[],"relativePath":"cpp-api-reference/classes/classWorkspace_1_1CoreOutPin.md","filePath":"cpp-api-reference/classes/classWorkspace_1_1CoreOutPin.md"}'),a={name:"cpp-api-reference/classes/classWorkspace_1_1CoreOutPin.md"},r=n(`<h1 id="workspace-coreoutpin" tabindex="-1">Workspace::CoreOutPin <a class="header-anchor" href="#workspace-coreoutpin" aria-label="Permalink to &quot;Workspace::CoreOutPin&quot;">​</a></h1><p><code>#include &quot;GUI/Workspace/Nodes/Basic/CorePins.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html">Workspace::CorePin</a>, <a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html">DIWNE::Pin</a>, <a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html">DIWNE::DiwneObject</a>, std::enable_shared_from_this&lt; DiwneObject &gt;</p><p>Inherited by <a href="/cpp-api-reference/classes/classWorkspace_1_1CoreOutPinMatrixMultiply.html">Workspace::CoreOutPinMatrixMultiply</a>, <a href="/cpp-api-reference/classes/classWorkspace_1_1DataOutPin.html">Workspace::DataOutPin</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CoreOutPin.html#function-coreoutpin">CoreOutPin</a></strong>(<a href="/cpp-api-reference/classes/classDIWNE_1_1Diwne.html">DIWNE::Diwne</a> &amp; diwne, <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#typedef-id">DIWNE::ID</a> const id, <a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Core::Pin</a> const &amp; pin, <a href="/cpp-api-reference/classes/classWorkspace_1_1CoreNode.html">CoreNode</a> &amp; node)</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CoreOutPin.html#function-content">content</a></strong>() override&lt;br&gt;Draw the output Pin: label and icon [float &gt;].</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CoreOutPin.html#function-updateconnectionpointdiwne">updateConnectionPointDiwne</a></strong>() override</td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html">Workspace::CorePin</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-corepin">CorePin</a></strong>(<a href="/cpp-api-reference/classes/classDIWNE_1_1Diwne.html">DIWNE::Diwne</a> &amp; diwne, <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#typedef-id">DIWNE::ID</a> const id, <a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Core::Pin</a> const &amp; pin, <a href="/cpp-api-reference/classes/classWorkspace_1_1CoreNode.html">CoreNode</a> &amp; node)</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-bypassfocusforinteractionaction">bypassFocusForInteractionAction</a></strong>() override</td></tr><tr><td><a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Core::Pin</a> const &amp;</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-getcorepin">getCorePin</a></strong>() const</td></tr><tr><td>int</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-getindex">getIndex</a></strong>() const</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceWorkspace.html#enum-pinkind">PinKind</a></td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-getkind">getKind</a></strong>() const</td></tr><tr><td><a href="/cpp-api-reference/classes/classWorkspace_1_1CoreNode.html">CoreNode</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-getnode">getNode</a></strong>()</td></tr><tr><td>const <a href="/cpp-api-reference/classes/classWorkspace_1_1CoreNode.html">CoreNode</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-getnode">getNode</a></strong>() const</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceCore.html#enum-evaluetype">Core::EValueType</a></td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-gettype">getType</a></strong>() const</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-isconnected">isConnected</a></strong>() const</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-popupcontent">popupContent</a></strong>() override&lt;br&gt;Content of popup menu raise on this object.</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-processconnectionprepared">processConnectionPrepared</a></strong>() override</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-processdrag">processDrag</a></strong>() override</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html">Workspace::CorePin</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td><a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#enum-icontype">DIWNE::IconType</a></td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#variable-m-icontype">m_iconType</a></strong></td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html">Workspace::CorePin</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>ImRect</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#variable-m-iconrectdiwne">m_iconRectDiwne</a></strong></td></tr><tr><td><a href="/cpp-api-reference/classes/classWorkspace_1_1CoreNode.html">CoreNode</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#variable-m-node">m_node</a></strong></td></tr><tr><td><a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Core::Pin</a> const &amp;</td><td><strong><a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#variable-m-pin">m_pin</a></strong></td></tr></tbody></table><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html">DIWNE::Pin</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-pin">Pin</a></strong>(<a href="/cpp-api-reference/classes/classDIWNE_1_1Diwne.html">DIWNE::Diwne</a> &amp; diwne, <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#typedef-id">DIWNE::ID</a> id, std::string const labelDiwne =&quot;DiwnePin&quot;)&lt;br&gt;Default constructor.</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-~pin">~Pin</a></strong>()&lt;br&gt;Default destructor.</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-allowprocesspin-pre-connectlink">allowProcessPin_Pre_ConnectLink</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-begin">begin</a></strong>() override</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-bypasspinlinkconnectionpreparedaction">bypassPinLinkConnectionPreparedAction</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-end">end</a></strong>() override</td></tr><tr><td>virtual <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#enum-diwneaction">DIWNE::DiwneAction</a></td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-getdragactiontype">getDragActionType</a></strong>() const</td></tr><tr><td>virtual <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#enum-diwneaction">DIWNE::DiwneAction</a></td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-getholdactiontype">getHoldActionType</a></strong>() const</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#typedef-id">DIWNE::ID</a> const</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-getid">getId</a></strong>() const</td></tr><tr><td>virtual const ImVec2 &amp;</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-getlinkconnectionpointdiwne">getLinkConnectionPointDiwne</a></strong>()</td></tr><tr><td>virtual ImRect</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-getrectdiwne">getRectDiwne</a></strong>() const override</td></tr><tr><td>virtual <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#enum-diwneaction">DIWNE::DiwneAction</a></td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-gettouchactiontype">getTouchActionType</a></strong>() const</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-processconnectionprepared">processConnectionPrepared</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-processdrag">processDrag</a></strong>() override</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-processinteractionsalways">processInteractionsAlways</a></strong>() override</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-processpin-pre-connectlinkdiwne">processPin_Pre_ConnectLinkDiwne</a></strong>()&lt;br&gt;Wrapper is run when new link is created and goal pin is hovered but action not released yet.</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-setconnectionpointdiwne">setConnectionPointDiwne</a></strong>(const ImVec2 value)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-updatesizes">updateSizes</a></strong>() override</td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html">DIWNE::Pin</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>ImVec2</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#variable-m-connectionpointdiwne">m_connectionPointDiwne</a></strong></td></tr><tr><td>ImRect</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#variable-m-pinrectdiwne">m_pinRectDiwne</a></strong> &lt;br&gt;PIN bounding rectangle.</td></tr></tbody></table><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html">DIWNE::DiwneObject</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-diwneobject">DiwneObject</a></strong>(<a href="/cpp-api-reference/classes/classDIWNE_1_1Diwne.html">DIWNE::Diwne</a> &amp; diwne, <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#typedef-id">DIWNE::ID</a> id, std::string const labelDiwne)&lt;br&gt;Constructor.</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-~diwneobject">~DiwneObject</a></strong>() =default</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-aftercontent">afterContent</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-aftercontentdiwne">afterContentDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-afterend">afterEnd</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-afterenddiwne">afterEndDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowdrawing">allowDrawing</a></strong>()&lt;br&gt;Decision maker whether object should be drawn (not outside of window for example)</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowinteraction">allowInteraction</a></strong>()&lt;br&gt;Decision maker whether object can interact (not include content elements)</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowprocessdrag">allowProcessDrag</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowprocessfocused">allowProcessFocused</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowprocessfocusedforinteraction">allowProcessFocusedForInteraction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowprocesshold">allowProcessHold</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowprocessraisepopup">allowProcessRaisePopup</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowprocessselect">allowProcessSelect</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowprocessunhold">allowProcessUnhold</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-allowprocessunselect">allowProcessUnselect</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-beforebegin">beforeBegin</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-beforebegindiwne">beforeBeginDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-beforecontent">beforeContent</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-beforecontentdiwne">beforeContentDiwne</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-begin">begin</a></strong>() =0</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-bypassdragaction">bypassDragAction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-bypassfocusaction">bypassFocusAction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-bypassfocusforinteractionaction">bypassFocusForInteractionAction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-bypassholdaction">bypassHoldAction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-bypassraisepopupaction">bypassRaisePopupAction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-bypassselectaction">bypassSelectAction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-bypasstouchaction">bypassTouchAction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-bypassunholdaction">bypassUnholdAction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-bypassunselectaction">bypassUnselectAction</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-contentdiwne">contentDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-drawdiwne">drawDiwne</a></strong>(<a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#enum-drawmode">DrawMode</a> drawMode =DrawMode::Interacting)&lt;br&gt;Main function for drawing object and deal with interactions.</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-end">end</a></strong>() =0</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-finalize">finalize</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-finalizediwne">finalizeDiwne</a></strong>()</td></tr><tr><td>virtual <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#enum-diwneaction">DIWNE::DiwneAction</a></td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-getdragactiontype">getDragActionType</a></strong>() const =0</td></tr><tr><td>virtual <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#enum-diwneaction">DIWNE::DiwneAction</a></td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-getholdactiontype">getHoldActionType</a></strong>() const =0</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#typedef-id">DIWNE::ID</a> const</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-getiddiwne">getIdDiwne</a></strong>() const</td></tr><tr><td>virtual ImRect</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-getrectdiwne">getRectDiwne</a></strong>() const =0</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-getselectable">getSelectable</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-getselected">getSelected</a></strong>() const&lt;br&gt;Getter of selection state.</td></tr><tr><td>virtual <a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#enum-diwneaction">DIWNE::DiwneAction</a></td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-gettouchactiontype">getTouchActionType</a></strong>() const =0</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-initialize">initialize</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-initializediwne">initializeDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-interactionbegininlastdraw">interactionBeginInLastDraw</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-interactionendinlastdraw">interactionEndInLastDraw</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-popupcontent">popupContent</a></strong>()&lt;br&gt;Content of popup menu raise on this object.</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processdrag">processDrag</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processdragdiwne">processDragDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processfocused">processFocused</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processfocuseddiwne">processFocusedDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processfocusedforinteraction">processFocusedForInteraction</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processfocusedforinteractiondiwne">processFocusedForInteractionDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processhold">processHold</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processholddiwne">processHoldDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processinteractions">processInteractions</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processinteractionsalways">processInteractionsAlways</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processinteractionsdiwne">processInteractionsDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processraisepopupdiwne">processRaisePopupDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processselect">processSelect</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processselectdiwne">processSelectDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processshowpopupdiwne">processShowPopupDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processunhold">processUnhold</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processunholddiwne">processUnholdDiwne</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processunselect">processUnselect</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-processunselectdiwne">processUnselectDiwne</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-setselectable">setSelectable</a></strong>(bool const selectable)</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-setselected">setSelected</a></strong>(bool const selected)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-showtooltiplabel">showTooltipLabel</a></strong>(std::string const &amp; label, ImColor const &amp;&amp; color)&lt;br&gt;Show a colored text for example for immediate hints.</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#function-updatesizes">updateSizes</a></strong>()</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html">DIWNE::DiwneObject</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td><a href="/cpp-api-reference/classes/classDIWNE_1_1Diwne.html">DIWNE::Diwne</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-diwne">diwne</a></strong></td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#enum-drawmode">DrawMode</a></td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-drawmode">m_drawMode</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-focused">m_focused</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-focusedforinteraction">m_focusedForInteraction</a></strong></td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceDIWNE.html#typedef-id">DIWNE::ID</a> const</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-iddiwne">m_idDiwne</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-inner-interaction-happen">m_inner_interaction_happen</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-inner-interaction-happen-previous-draw">m_inner_interaction_happen_previous_draw</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-isactive">m_isActive</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-isdragged">m_isDragged</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-isheld">m_isHeld</a></strong></td></tr><tr><td>std::string const</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-labeldiwne">m_labelDiwne</a></strong></td></tr><tr><td>std::string const</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-popupiddiwne">m_popupIDDiwne</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-selectable">m_selectable</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html#variable-m-selected">m_selected</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-coreoutpin" tabindex="-1">function CoreOutPin <a class="header-anchor" href="#function-coreoutpin" aria-label="Permalink to &quot;function CoreOutPin&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">CoreOutPin</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">DIWNE</span><span style="color:#E1E4E8;">::Diwne </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> diwne,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">DIWNE</span><span style="color:#E1E4E8;">::ID </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> id,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Core</span><span style="color:#E1E4E8;">::Pin </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> pin,</span></span>
<span class="line"><span style="color:#E1E4E8;">    CoreNode </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> node</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">CoreOutPin</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">DIWNE</span><span style="color:#24292E;">::Diwne </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> diwne,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">DIWNE</span><span style="color:#24292E;">::ID </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> id,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Core</span><span style="color:#24292E;">::Pin </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> pin,</span></span>
<span class="line"><span style="color:#24292E;">    CoreNode </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> node</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-content" tabindex="-1">function content <a class="header-anchor" href="#function-content" aria-label="Permalink to &quot;function content&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">content</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">content</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">override</span></span></code></pre></div><p>Draw the output Pin: label and icon [float &gt;].</p><p><strong>Return</strong>: true if value changed</p><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classWorkspace_1_1CorePin.html#function-content">Workspace::CorePin::content</a></p><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classWorkspace_1_1DataOutPin.html#function-content">Workspace::DataOutPin::content</a></p><h3 id="function-updateconnectionpointdiwne" tabindex="-1">function updateConnectionPointDiwne <a class="header-anchor" href="#function-updateconnectionpointdiwne" aria-label="Permalink to &quot;function updateConnectionPointDiwne&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">updateConnectionPointDiwne</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">updateConnectionPointDiwne</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">override</span></span></code></pre></div><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classDIWNE_1_1Pin.html#function-updateconnectionpointdiwne">DIWNE::Pin::updateConnectionPointDiwne</a></p><p>intended to use when Pin is drawn (use properties dependent on drawing)</p><ul><li>setConnectionPointDiwne is &quot;hard&quot; setting</li></ul><hr><p>Updated on 2024-10-15 at 20:41:58 +0000</p>`,37),c=[r];function o(i,l,p,d,f,h){return t(),s("div",null,c)}const _=e(a,[["render",o]]);export{g as __pageData,_ as default};
