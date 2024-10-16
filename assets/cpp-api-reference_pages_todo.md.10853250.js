import{_ as e,o as a,c as r,Q as s}from"./chunks/framework.0f749972.js";const d=JSON.parse('{"title":"Todo List","description":"","frontmatter":{"title":"Todo List"},"headers":[],"relativePath":"cpp-api-reference/pages/todo.md","filePath":"cpp-api-reference/pages/todo.md"}'),l={name:"cpp-api-reference/pages/todo.md"},i=s('<h1 id="todo-list" tabindex="-1">Todo List <a class="header-anchor" href="#todo-list" aria-label="Permalink to &quot;Todo List&quot;">​</a></h1><p>Class <a href="/cpp-api-reference/classes/structChoiceTask.html">ChoiceTask</a></p><ul><li>Not implemented!</li></ul><p>Member <a href="/cpp-api-reference/classes/classCommand.html#function-addlistener">Command&lt; Type, Args &gt;::addListener</a> (Callback function)</p><ul><li>Check how std::bind handle <code>this</code> reference.</li></ul><p>Member <a href="/cpp-api-reference/classes/structConfig.html#variable-def-desc">Config::DEF_DESC</a></p><ul><li>Complete the help: Fill in all missing key combinations into the help window in config.h Update to contain all key bindings from tabsConfig.cpp</li></ul><p>Member <a href="/cpp-api-reference/classes/structConfig.html#variable-info-text">Config::INFO_TEXT</a></p><ul><li>Add year and note of the continual development.</li></ul><p>Member <a href="/cpp-api-reference/classes/structConfig.html#variable-link-color">Config::LINK_COLOR</a></p><ul><li>Move to the config structure</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Cycle.html#function-updatevalues">Core::Cycle::updateValues</a> (int inputIndex) override</p><ul><li>(PF) Why does it ignore the input pin????</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Data.html#function-getvalue">Core::Data::getValue</a> () const</p><ul><li>MH Refactor</li></ul><p>Member <a href="/cpp-api-reference/namespaces/namespaceCore.html#enum-evaluestate">Core::EValueState</a></p><ul><li>PF:- proc tohle neni po bitech? SynergiesBit = 0x0001, ///&lt; 001 EditableBit = 0x0002, ///&lt; 010 PossibleNextBit = 0x0004, ///&lt; 100 ...</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1GraphManager.html#function-createsequence">Core::GraphManager::createSequence</a> ()</p><ul><li>MH Temporary workaround:</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1GraphManager.html#function-getparent">Core::GraphManager::getParent</a> (const Ptr&lt; Node &gt; &amp;node, size_t index=0)</p><ul><li>Move me to NodeUtils.</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Model.html#function-resetmodelposition">Core::Model::resetModelPosition</a> ()</p><ul><li></li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-finalize">Core::Node::finalize</a> ()</p><ul><li>MH: Remove, use destructor directly.</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getinternaldata">Core::Node::getInternalData</a> (size_t index=0)</p><ul><li>Make this function non public.</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Node.html#variable-m-operatorstate">Core::Node::m_OperatorState</a></p><ul><li>Break chain <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> -&gt; <a href="/cpp-api-reference/classes/classCore_1_1Transform.html">Transform</a>. This is pin related, move to <a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Pin</a> class.</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-spreadsignal">Core::Node::spreadSignal</a> ()</p><ul><li>Does not use operators for calling each follower just once MH Correct the owner of the pin.</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Transform.html#function-getdefaultvalue">Core::Transform::getDefaultValue</a> (const std::string &amp;name) const</p><ul><li>rename to getCurrentValue?</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Transform.html#function-isvalid">Core::Transform::isValid</a> () const =0</p><ul><li>isValid checks matrix AND parameters from the SetValues LOD (Default values)</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Transform.html#function-nullsequence">Core::Transform::nullSequence</a> ()</p><ul><li>MH these should not be public.</li></ul><p>Member <a href="/cpp-api-reference/classes/classCore_1_1Transform.html#function-resetmatrixfromdefaults">Core::Transform::resetMatrixFromDefaults</a> ()=0</p><ul><li>For synergies, it has to be resolved. Most probably, it should leave the synergies unchanged. <ul><li>for Scale When setting X value in non-uniform scale -&gt; this switch to uniform scale (due to enable synergies)</li></ul></li></ul><p>Member <a href="/cpp-api-reference/files/Logger_8cpp.html#define-default-log-pattern">DEFAULT_LOG_PATTERN</a></p><ul><li>Test JustPressed keys in <a href="/cpp-api-reference/classes/classApplication.html">Application</a> and set set logging options there.</li></ul><p>Class <a href="/cpp-api-reference/classes/classDescriptionDialog.html">DescriptionDialog</a></p><ul><li>Create scene description here. Make text resizable. Make open scene unsaved after text editing.</li></ul><p>Member <a href="/cpp-api-reference/classes/classDIWNE_1_1Diwne.html#function-applyzoomscaling">DIWNE::Diwne::applyZoomScaling</a> ()</p><ul><li></li></ul><p>Member <a href="/cpp-api-reference/classes/classDIWNE_1_1Diwne.html#variable-m-popupdrawn">DIWNE::Diwne::m_popupDrawn</a></p><ul><li>maybe unused when every object is drawn just one time</li></ul><p>Member <a href="/cpp-api-reference/classes/classDIWNE_1_1Diwne.html#variable-m-tooltipdrawn">DIWNE::Diwne::m_tooltipDrawn</a></p><ul><li>maybe unused when every object is drawn just one time</li></ul><p>Member <a href="/cpp-api-reference/classes/structInputBindings.html#function-setactionkey">InputBindings::setActionKey</a> (const char *name, <a href="/cpp-api-reference/classes/classKeys.html#enum-code">Keys::Code</a> code, ModifiersList modifiers=<a href="/cpp-api-reference/files/InputManager_8h.html#using-modifierslist">ModifiersList()</a>)</p><ul><li>MH Check for conflicts.</li></ul><p>Member <a href="/cpp-api-reference/classes/classInputManager.html#variable-m-keymap">InputManager::m_keyMap</a></p><ul><li>passed to all handlers, but probably not used</li></ul><p>Class <a href="/cpp-api-reference/classes/structInputTask.html">InputTask</a></p><ul><li>Not implemented!</li></ul><p>Member <a href="/cpp-api-reference/classes/classLogWindow.html#function-render">LogWindow::render</a> () override</p><ul><li>MH test code, remove</li></ul><p>File <a href="/cpp-api-reference/files/Math_8h#file-math.h">Math.h</a></p><ul><li>Test the appropriate factors</li></ul><p>Class <a href="/cpp-api-reference/classes/structMultiChoiceTask.html">MultiChoiceTask</a></p><ul><li>Not implemented!</li></ul><p>Member <a href="/cpp-api-reference/files/TutorialWindow_8cpp.html#variable-next-button-size-x">NEXT_BUTTON_SIZE_X</a></p><ul><li>If you want to use ImGui Markdown run $ git submodule add <a href="https://github.com/Mizumaky/imgui_markdown_extended" target="_blank" rel="noreferrer">https://github.com/Mizumaky/imgui_markdown_extended</a> Depencencies/imgui_markdown.</li></ul><p>Member <a href="/cpp-api-reference/classes/classStateManager.html#function-getpossibleredoscount">StateManager::getPossibleRedosCount</a> () const</p><ul><li>Test me!</li></ul><p>Member <a href="/cpp-api-reference/classes/classTheme.html#using-categorynames">Theme::CategoryNames</a></p><ul><li>MH - P0919R2 Heterogeneous lookup for unordered containers, C++2a (std::unordered_map cannot be used).</li></ul><p>Member <a href="/cpp-api-reference/classes/classTheme.html#function-initnames">Theme::initNames</a> ()</p><ul><li>This will be removed. This will be removed. This will be removed. This will be removed. This will be removed. This will be removed. This will be removed.</li></ul><p>File <a href="/cpp-api-reference/files/TransformImpl_8h#file-transformimpl.h">TransformImpl.h</a></p><ul><li>: <ul><li>setValue(mat) does not respect the synergies - It is used in FreeManipulator, <a href="/cpp-api-reference/classes/classSerializationVisitor.html">SerializationVisitor</a>.</li></ul></li></ul><p>setDefaultValue - now just sets something, but omits the synergies. For quat it is done in resetMatrixFromDefault(). Is it the right place?</p><ul><li>where is it used? World/Components/FreeManipulator.cpp::561 <a href="/cpp-api-reference/files/SerializationVisitor_8cpp#file-serializationvisitor.cpp">State/SerializationVisitor.cpp</a>:445</li></ul><p><a href="/cpp-api-reference/classes/classUI_1_1ViewportWindow.html">UI::ViewportWindow</a> Member <a href="/cpp-api-reference/classes/classUI_1_1ViewportWindow.html#function-viewportwindow">UI::ViewportWindow::ViewportWindow</a> (bool show, <a href="/cpp-api-reference/classes/classVp_1_1Viewport.html">Vp::Viewport</a> *viewport) MH This is example code, it can be removed anytime.</p><p><a href="/cpp-api-reference/namespaces/namespaceWorkspace.html">Workspace</a> Member <a href="/cpp-api-reference/namespaces/namespaceWorkspace.html#variable-pincolorbackground">Workspace::PinColorBackground</a>Remove these.</p><p><a href="/cpp-api-reference/classes/classWorkspace_1_1TransformationBase.html">Workspace::TransformationBase</a> Member <a href="/cpp-api-reference/classes/classWorkspace_1_1TransformationBase.html#function-drawdatafull">Workspace::TransformationBase::drawDataFull</a> () see #111</p><p><a href="/cpp-api-reference/classes/classWorkspace_1_1WorkspaceDiwne.html">Workspace::WorkspaceDiwne</a> Member <a href="/cpp-api-reference/classes/classWorkspace_1_1WorkspaceDiwne.html#function-content">Workspace::WorkspaceDiwne::content</a> () override Cannot draw with empty workspace -&gt; <code>m_channelSplitter.Split(ImGui::GetWindowDrawList(), node_count );</code> exception.</p><p>see #111, wrongly computed m_takeSnap value.</p><hr><p>Updated on 2024-10-16 at 16:44:51 +0000</p>',80),t=[i];function p(o,n,c,u,m,f){return a(),r("div",null,t)}const _=e(l,[["render",p]]);export{d as __pageData,_ as default};