import{_ as e,o as t,c as a,Q as s}from"./chunks/framework.0f749972.js";const g=JSON.parse('{"title":"Core::Model","description":"","frontmatter":{"title":"Core::Model"},"headers":[],"relativePath":"cpp-api-reference/classes/classCore_1_1Model.md","filePath":"cpp-api-reference/classes/classCore_1_1Model.md"}'),r={name:"cpp-api-reference/classes/classCore_1_1Model.md"},o=s(`<h1 id="core-model" tabindex="-1">Core::Model <a class="header-anchor" href="#core-model" aria-label="Permalink to &quot;Core::Model&quot;">​</a></h1><p><code>#include &quot;Core/Nodes/Model.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Core::Node</a>, std::enable_shared_from_this&lt; Node &gt;</p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Model.html#function-model">Model</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Model.html#function-onunpluginput">onUnplugInput</a></strong>(size_t index) override</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Model.html#function-resetmodelposition">resetModelPosition</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Model.html#function-updatevalues">updateValues</a></strong>(int inputIndex =0) override&lt;br&gt;Computes new values of outputs based on inputs.</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>glm::mat4</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Model.html#variable-m-modelmatrix">m_modelMatrix</a></strong> &lt;br&gt;Latest model matrix value.</td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Core::Node</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-adddeletecallback">addDeleteCallback</a></strong>(std::function&lt; void(<a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> <em>)&gt; callback)&lt;br&gt; The callback parameters: Node</em> = node that has been deleted</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-addplugcallback">addPlugCallback</a></strong>(std::function&lt; void(<a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> *, <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> *, size_t, size_t)&gt; callback)&lt;br&gt;Registers a callback that gets called when the node&#39;s output pins get plugged in.</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-addunplugcallback">addUnplugCallback</a></strong>(std::function&lt; void(<a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> *, <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> *, size_t, size_t)&gt; callback)&lt;br&gt;Registers a callback that gets called when the node&#39;s input pins get unplugged.</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-addupdatecallback">addUpdateCallback</a></strong>(std::function&lt; void(<a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> *)&gt; callback)&lt;br&gt;Registers a callback that gets called on any <a href="./.html">updateValues()</a> call.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-appendchildnode">appendChildNode</a></strong>(<a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> &gt; node)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-areallinputsplugged">areAllInputsPlugged</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-areallinputsunplugged">areAllInputsUnplugged</a></strong>() const</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-areinputsplugged">areInputsPlugged</a></strong>(int numInputs)</td></tr><tr><td>template &lt;typename T &amp;gt; &lt;br&gt;<a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; T &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-as">as</a></strong>()</td></tr><tr><td>const <a href="/cpp-api-reference/classes/classCore_1_1Data.html">Data</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-data">data</a></strong>(size_t index =0) const&lt;br&gt;Get <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> contents, read only.</td></tr><tr><td><a href="/cpp-api-reference/classes/classCore_1_1Data.html">Data</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-datamut">dataMut</a></strong>(size_t index)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-finalize">finalize</a></strong>()&lt;br&gt;Prepares node for its destruction, after that the destructor can be called.</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceCore.html#typedef-id">ID</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getid">getId</a></strong>() const</td></tr><tr><td><a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Pin</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getinput">getInput</a></strong>(size_t i)</td></tr><tr><td>std::vector&lt; <a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Pin</a> &gt; &amp;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getinputpins">getInputPins</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/classes/classCore_1_1Data.html">Data</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getinternaldata">getInternalData</a></strong>(size_t index =0)&lt;br&gt;Get data storage for read and write purposes.</td></tr><tr><td>const char *</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getlabel">getLabel</a></strong>() const</td></tr><tr><td>const <a href="/cpp-api-reference/classes/structCore_1_1Operation.html">Operation</a> *</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getoperation">getOperation</a></strong>() const</td></tr><tr><td><a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Pin</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getoutput">getOutput</a></strong>(size_t i)</td></tr><tr><td>std::vector&lt; <a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Pin</a> &gt; &amp;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getoutputpins">getOutputPins</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getowner">getOwner</a></strong>() const&lt;br&gt;Get direct owner of this node.</td></tr><tr><td><a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getptr">getPtr</a></strong>()&lt;br&gt;Get reference to this node instance.</td></tr><tr><td><a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getrootowner">getRootOwner</a></strong>()&lt;br&gt;Get the topmost owner of this node.</td></tr><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getsignature">getSignature</a></strong>()&lt;br&gt;&quot;{node type} #{node ID}&quot;</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceCore.html#enum-evaluestate">EValueState</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-getstate">getState</a></strong>(size_t pinIndex =0)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-init">init</a></strong>()&lt;br&gt;Initialize node id, inputs, and outputs according to the preset node type.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-notifyowner">notifyOwner</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-oninit">onInit</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-operator!=">operator!=</a></strong>(const <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> &amp; other) const</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-operator==">operator==</a></strong>(const <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> &amp; other) const</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceCore.html#enum-enodeplugresult">ENodePlugResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-plug">plug</a></strong>(const <a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> &gt; &amp; childNode, unsigned fromIndex, unsigned toIndex)&lt;br&gt;Connect this (parent) node output to the <code>childNode</code> input.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-pulse">pulse</a></strong>(size_t index)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-receivesignal">receiveSignal</a></strong>(int inputIndex)&lt;br&gt;Implements the operator reaction to the change of its <em>inputIndex</em> input.</td></tr><tr><td>virtual <a href="/cpp-api-reference/classes/structCore_1_1SetValueResult.html">SetValueResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setvalue">setValue</a></strong>(const glm::mat4 &amp; mat)</td></tr><tr><td>virtual <a href="/cpp-api-reference/classes/structCore_1_1SetValueResult.html">SetValueResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setvalue">setValue</a></strong>(const glm::mat4 &amp; mat, const <a href="/cpp-api-reference/classes/classCore_1_1DataMap.html">DataMap</a> &amp; map)&lt;br&gt;Smart set function, used with constrained transformation for value checking.</td></tr><tr><td>virtual <a href="/cpp-api-reference/classes/structCore_1_1SetValueResult.html">SetValueResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setvalue">setValue</a></strong>(const glm::quat &amp; q)</td></tr><tr><td>virtual <a href="/cpp-api-reference/classes/structCore_1_1SetValueResult.html">SetValueResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setvalue">setValue</a></strong>(const glm::vec3 &amp; vec)</td></tr><tr><td>virtual <a href="/cpp-api-reference/classes/structCore_1_1SetValueResult.html">SetValueResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setvalue">setValue</a></strong>(const glm::vec4 &amp; vec)</td></tr><tr><td>template &lt;typename T &amp;gt; &lt;br&gt;<a href="/cpp-api-reference/classes/structCore_1_1SetValueResult.html">SetValueResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setvalue">setValue</a></strong>(const T &amp; value, unsigned index)</td></tr><tr><td>virtual <a href="/cpp-api-reference/classes/structCore_1_1SetValueResult.html">SetValueResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setvalue">setValue</a></strong>(float val)&lt;br&gt;Set the value of the node.</td></tr><tr><td>virtual <a href="/cpp-api-reference/classes/structCore_1_1SetValueResult.html">SetValueResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setvalue">setValue</a></strong>(float val, glm::ivec2 coords)</td></tr><tr><td>virtual <a href="/cpp-api-reference/classes/structCore_1_1SetValueResult.html">SetValueResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setvalue">setValue</a></strong>(void * ptr)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-spreadsignal">spreadSignal</a></strong>()&lt;br&gt;Spread signal to all outputs.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-spreadsignal">spreadSignal</a></strong>(size_t outIndex)&lt;br&gt;Spread signal to the selected output <em>outIndex</em> only.</td></tr></tbody></table><p><strong>Protected Functions inherited from <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Core::Node</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-node">Node</a></strong>(const <a href="/cpp-api-reference/classes/structCore_1_1Operation.html">Operation</a> * operation)&lt;br&gt;<a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> is never constructed directory.</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-~node">~Node</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceCore.html#enum-enodeplugresult">ENodePlugResult</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-isplugcorrect">isPlugCorrect</a></strong>(const <a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Pin</a> &amp; input, const <a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Pin</a> &amp; output)</td></tr><tr><td>template &lt;typename T &amp;gt; &lt;br&gt;void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setinternalvalue">setInternalValue</a></strong>(const T &amp; value, size_t index =0)&lt;br&gt;Sets the node value without validation.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-setinternalvalue">setInternalValue</a></strong>(float value, glm::ivec2 coordinates, size_t index =0)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-shouldpulse">shouldPulse</a></strong>(size_t inputIndex, size_t updatedInputIndex)&lt;br&gt;Tests if the <code>updatedInputIndex</code> should be pulsed.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-triggerdeletecallback">triggerDeleteCallback</a></strong>(<a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> * node)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-triggerplugcallback">triggerPlugCallback</a></strong>(<a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> * fromNode, <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> * toNode, size_t fromIndex, size_t toIndex)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-triggerunplugcallback">triggerUnplugCallback</a></strong>(<a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> * fromNode, <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> * toNode, size_t fromIndex, size_t toIndex)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-triggerupdatecallback">triggerUpdateCallback</a></strong>(<a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> * node)</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Core::Node</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>std::vector&lt; <a href="/cpp-api-reference/namespaces/namespaceCore.html#enum-evaluestate">EValueState</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#variable-m-operatorstate">m_OperatorState</a></strong></td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Core::Node</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>std::vector&lt; <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> * &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#variable-m-children">m_children</a></strong> &lt;br&gt;Nested nodes.</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespaceCore.html#typedef-id">ID</a></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#variable-m-id">m_id</a></strong></td></tr><tr><td>std::vector&lt; <a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Pin</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#variable-m-inputs">m_inputs</a></strong> &lt;br&gt;Inputs of the box: Input tabs with glyphs.</td></tr><tr><td>std::vector&lt; <a href="/cpp-api-reference/classes/classCore_1_1Data.html">Data</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#variable-m-internaldata">m_internalData</a></strong> &lt;br&gt;Results of operations.</td></tr><tr><td>const <a href="/cpp-api-reference/classes/structCore_1_1Operation.html">Operation</a> *</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#variable-m-operation">m_operation</a></strong> &lt;br&gt;<a href="./.html">Operator</a> node properties.</td></tr><tr><td>std::vector&lt; <a href="/cpp-api-reference/classes/classCore_1_1Pin.html">Pin</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#variable-m-outputs">m_outputs</a></strong> &lt;br&gt;Outputs of the box: output tabs with glyphs.</td></tr><tr><td><a href="/cpp-api-reference/classes/classCore_1_1Node.html">Node</a> *</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#variable-m-owner">m_owner</a></strong> &lt;br&gt;Owner of the node, used in complex type of nodes, such as sequence or camera.</td></tr></tbody></table><p><strong>Friends inherited from <a href="/cpp-api-reference/classes/classCore_1_1Node.html">Core::Node</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>class</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#friend-graphmanager">GraphManager</a></strong></td></tr><tr><td>class</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1Node.html#friend-pin">Pin</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-model" tabindex="-1">function Model <a class="header-anchor" href="#function-model" aria-label="Permalink to &quot;function Model&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Model</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Model</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-onunpluginput" tabindex="-1">function onUnplugInput <a class="header-anchor" href="#function-onunpluginput" aria-label="Permalink to &quot;function onUnplugInput&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">onUnplugInput</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">size_t</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">index</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">onUnplugInput</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">size_t</span><span style="color:#24292E;"> </span><span style="color:#E36209;">index</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">override</span></span></code></pre></div><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-onunpluginput">Core::Node::onUnplugInput</a></p><h3 id="function-resetmodelposition" tabindex="-1">function resetModelPosition <a class="header-anchor" href="#function-resetmodelposition" aria-label="Permalink to &quot;function resetModelPosition&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">resetModelPosition</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">resetModelPosition</span><span style="color:#24292E;">()</span></span></code></pre></div><p><strong>Todo</strong>:</p><h3 id="function-updatevalues" tabindex="-1">function updateValues <a class="header-anchor" href="#function-updatevalues" aria-label="Permalink to &quot;function updateValues&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">updateValues</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">int</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">inputIndex</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">updateValues</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">int</span><span style="color:#24292E;"> </span><span style="color:#E36209;">inputIndex</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">override</span></span></code></pre></div><p>Computes new values of outputs based on inputs.</p><p><strong>Parameters</strong>:</p><ul><li><strong>inputIndex</strong> Index of the modified input.</li></ul><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-updatevalues">Core::Node::updateValues</a></p><p>Encodes the box function by updating the output values for a given <em>inputIndex</em> - fired by <a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-receivesignal">receiveSignal()</a>.</p><p>The base class method calls update callbacks registered with <a href="/cpp-api-reference/classes/classCore_1_1Node.html#function-addupdatecallback">addUpdateCallback()</a>. So derived methods should also call their base method if they want to inform any outside observers.</p><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-modelmatrix" tabindex="-1">variable m_modelMatrix <a class="header-anchor" href="#variable-m-modelmatrix" aria-label="Permalink to &quot;variable m_modelMatrix&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::mat4 m_modelMatrix {};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::mat4 m_modelMatrix {};</span></span></code></pre></div><p>Latest model matrix value.</p><hr><p>Updated on 2024-10-15 at 20:41:59 +0000</p>`,41),n=[o];function l(c,p,d,i,h,u){return t(),a("div",null,n)}const m=e(r,[["render",l]]);export{g as __pageData,m as default};
