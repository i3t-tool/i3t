import{_ as s,o as a,c as n,Q as e}from"./chunks/framework.0f749972.js";const E=JSON.parse('{"title":"Core::Pin","description":"","frontmatter":{"title":"Core::Pin","summary":"Pin used for connecting nodes."},"headers":[],"relativePath":"cpp-api-reference/classes/classCore_1_1Pin.md","filePath":"cpp-api-reference/classes/classCore_1_1Pin.md"}'),p={name:"cpp-api-reference/classes/classCore_1_1Pin.md"},o=e(`<h1 id="core-pin" tabindex="-1">Core::Pin <a class="header-anchor" href="#core-pin" aria-label="Permalink to &quot;Core::Pin&quot;">​</a></h1><p><a href="./.html">Pin</a> used for connecting nodes. <a href="#detailed-description">More...</a></p><p><code>#include &quot;Core/Nodes/Pin.h&quot;</code></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-pin">Pin</a></strong>(<a href="/cpp-api-reference/namespaces/namespacecore.html#enum-evaluetype">EValueType</a> valueType, bool isInput, <a href="/cpp-api-reference/classes/classcore_1_1node.html">Node</a> &amp; owner, int index)</td></tr><tr><td>const <a href="/cpp-api-reference/classes/classcore_1_1data.html">Data</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-data">data</a></strong>() const&lt;br&gt;Get stored data based on pin type.</td></tr><tr><td><a href="/cpp-api-reference/classes/classcore_1_1data.html">Data</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-datamut">dataMut</a></strong>()</td></tr><tr><td>const std::string &amp;</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-getlabel">getLabel</a></strong>() const</td></tr><tr><td>const std::vector&lt; <a href="/cpp-api-reference/classes/classcore_1_1pin.html">Pin</a> * &gt; &amp;</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-getoutcomponents">getOutComponents</a></strong>() const</td></tr><tr><td><a href="/cpp-api-reference/files/types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classcore_1_1node.html">Node</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-getowner">getOwner</a></strong>() const</td></tr><tr><td>const <a href="/cpp-api-reference/classes/classcore_1_1pin.html">Pin</a> *</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-getparentpin">getParentPin</a></strong>() const</td></tr><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-getsignature">getSignature</a></strong>() const&lt;br&gt;&quot;pin {index} of {node signature}&quot;</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-isdisabled">isDisabled</a></strong>() const</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-ispluggedin">isPluggedIn</a></strong>() const&lt;br&gt;Query if input of this object is plugged to any parent output.</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-isrendered">isRendered</a></strong>() const&lt;br&gt;False in <a href="/cpp-api-reference/classes/classcore_1_1camera.html">Camera</a> P and V matrices ONLY.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-setdisabled">setDisabled</a></strong>(bool disabled)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-setrendered">setRendered</a></strong>(bool value)</td></tr><tr><td><a href="/cpp-api-reference/namespaces/namespacecore.html#enum-enodeplugresult">ENodePlugResult</a></td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#function-isplugcorrect">isPlugCorrect</a></strong>(const <a href="/cpp-api-reference/classes/classcore_1_1pin.html">Pin</a> &amp; input, const <a href="/cpp-api-reference/classes/classcore_1_1pin.html">Pin</a> &amp; output)</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td><a href="/cpp-api-reference/namespaces/namespacecore.html#typedef-id">ID</a></td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#variable-id">Id</a></strong></td></tr><tr><td>const int</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#variable-index">Index</a></strong> &lt;br&gt;Index within a node.</td></tr><tr><td>const bool</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#variable-isinput">IsInput</a></strong> &lt;br&gt;<a href="/cpp-api-reference/classes/classcore_1_1pin.html">Pin</a> type.</td></tr><tr><td><a href="/cpp-api-reference/classes/classcore_1_1node.html">Node</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#variable-owner">Owner</a></strong></td></tr><tr><td>const <a href="/cpp-api-reference/namespaces/namespacecore.html#enum-evaluetype">EValueType</a></td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#variable-valuetype">ValueType</a></strong></td></tr></tbody></table><h2 id="friends" tabindex="-1">Friends <a class="header-anchor" href="#friends" aria-label="Permalink to &quot;Friends&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>class</td><td><strong><a href="/cpp-api-reference/classes/classcore_1_1pin.html#friend-node">Node</a></strong></td></tr></tbody></table><h2 id="detailed-description" tabindex="-1">Detailed Description <a class="header-anchor" href="#detailed-description" aria-label="Permalink to &quot;Detailed Description&quot;">​</a></h2><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">class</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Core</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">Pin</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">class</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Core</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">Pin</span><span style="color:#24292E;">;</span></span></code></pre></div><p><a href="./.html">Pin</a> used for connecting nodes.</p><p>OperatorCurveTab from <a href="/cpp-api-reference/namespaces/namespacei3t.html">I3T</a> v1.</p><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-pin" tabindex="-1">function Pin <a class="header-anchor" href="#function-pin" aria-label="Permalink to &quot;function Pin&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Pin</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    EValueType valueType,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> isInput,</span></span>
<span class="line"><span style="color:#E1E4E8;">    Node </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> owner,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">int</span><span style="color:#E1E4E8;"> index</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Pin</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    EValueType valueType,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> isInput,</span></span>
<span class="line"><span style="color:#24292E;">    Node </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> owner,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">int</span><span style="color:#24292E;"> index</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-data" tabindex="-1">function data <a class="header-anchor" href="#function-data" aria-label="Permalink to &quot;function data&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Data</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">data</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Data</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">data</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><p>Get stored data based on pin type.</p><p><strong>Return</strong>: data storage owner by node connected to this input pin. If pin is output pin, it returns data storage of pin owner.</p><p><strong>Precondition</strong>: If pin is input, it must be plugged to some output pin.</p><h3 id="function-datamut" tabindex="-1">function dataMut <a class="header-anchor" href="#function-datamut" aria-label="Permalink to &quot;function dataMut&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Data</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">dataMut</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Data</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">dataMut</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getlabel" tabindex="-1">function getLabel <a class="header-anchor" href="#function-getlabel" aria-label="Permalink to &quot;function getLabel&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getLabel</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getLabel</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><h3 id="function-getoutcomponents" tabindex="-1">function getOutComponents <a class="header-anchor" href="#function-getoutcomponents" aria-label="Permalink to &quot;function getOutComponents&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">vector</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Pin</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getOutComponents</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">vector</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Pin</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getOutComponents</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><p><strong>Return</strong>: Input pins of connected nodes.</p><h3 id="function-getowner" tabindex="-1">function getOwner <a class="header-anchor" href="#function-getowner" aria-label="Permalink to &quot;function getOwner&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Node</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">getOwner</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Node</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">getOwner</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><h3 id="function-getparentpin" tabindex="-1">function getParentPin <a class="header-anchor" href="#function-getparentpin" aria-label="Permalink to &quot;function getParentPin&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Pin</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getParentPin</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Pin</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getParentPin</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><p><strong>Return</strong>: nullptr if input is not plugged in.</p><h3 id="function-getsignature" tabindex="-1">function getSignature <a class="header-anchor" href="#function-getsignature" aria-label="Permalink to &quot;function getSignature&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getSignature</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getSignature</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><p>&quot;pin {index} of {node signature}&quot;</p><h3 id="function-isdisabled" tabindex="-1">function isDisabled <a class="header-anchor" href="#function-isdisabled" aria-label="Permalink to &quot;function isDisabled&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">isDisabled</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">isDisabled</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><h3 id="function-ispluggedin" tabindex="-1">function isPluggedIn <a class="header-anchor" href="#function-ispluggedin" aria-label="Permalink to &quot;function isPluggedIn&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">isPluggedIn</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">isPluggedIn</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><p>Query if input of this object is plugged to any parent output.</p><p><strong>Return</strong>: True if plugged to parent, false if not.</p><h3 id="function-isrendered" tabindex="-1">function isRendered <a class="header-anchor" href="#function-isrendered" aria-label="Permalink to &quot;function isRendered&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">isRendered</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">isRendered</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><p>False in <a href="/cpp-api-reference/classes/classcore_1_1camera.html">Camera</a> P and V matrices ONLY.</p><p>Used to skip hidden values stored as pins.</p><h3 id="function-setdisabled" tabindex="-1">function setDisabled <a class="header-anchor" href="#function-setdisabled" aria-label="Permalink to &quot;function setDisabled&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setDisabled</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">disabled</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setDisabled</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#E36209;">disabled</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-setrendered" tabindex="-1">function setRendered <a class="header-anchor" href="#function-setrendered" aria-label="Permalink to &quot;function setRendered&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setRendered</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">value</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setRendered</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#E36209;">value</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-isplugcorrect" tabindex="-1">function isPlugCorrect <a class="header-anchor" href="#function-isplugcorrect" aria-label="Permalink to &quot;function isPlugCorrect&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ENodePlugResult</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">isPlugCorrect</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Pin</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">input</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Pin</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">output</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ENodePlugResult</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">isPlugCorrect</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Pin</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">input</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Pin</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">output</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-id" tabindex="-1">variable Id <a class="header-anchor" href="#variable-id" aria-label="Permalink to &quot;variable Id&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">ID Id;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">ID Id;</span></span></code></pre></div><h3 id="variable-index" tabindex="-1">variable Index <a class="header-anchor" href="#variable-index" aria-label="Permalink to &quot;variable Index&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">int</span><span style="color:#E1E4E8;"> Index </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">-</span><span style="color:#79B8FF;">1</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">int</span><span style="color:#24292E;"> Index </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">-</span><span style="color:#005CC5;">1</span><span style="color:#24292E;">;</span></span></code></pre></div><p>Index within a node.</p><h3 id="variable-isinput" tabindex="-1">variable IsInput <a class="header-anchor" href="#variable-isinput" aria-label="Permalink to &quot;variable IsInput&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> IsInput;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> IsInput;</span></span></code></pre></div><p><a href="/cpp-api-reference/classes/classcore_1_1pin.html">Pin</a> type.</p><h3 id="variable-owner" tabindex="-1">variable Owner <a class="header-anchor" href="#variable-owner" aria-label="Permalink to &quot;variable Owner&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">Node </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> Owner;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">Node </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> Owner;</span></span></code></pre></div><h3 id="variable-valuetype" tabindex="-1">variable ValueType <a class="header-anchor" href="#variable-valuetype" aria-label="Permalink to &quot;variable ValueType&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> EValueType ValueType </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">EValueType</span><span style="color:#E1E4E8;">::Pulse;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> EValueType ValueType </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">EValueType</span><span style="color:#24292E;">::Pulse;</span></span></code></pre></div><h2 id="friends-1" tabindex="-1">Friends <a class="header-anchor" href="#friends-1" aria-label="Permalink to &quot;Friends&quot;">​</a></h2><h3 id="friend-node" tabindex="-1">friend Node <a class="header-anchor" href="#friend-node" aria-label="Permalink to &quot;friend Node&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">friend</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">class</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Node</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Node</span><span style="color:#E1E4E8;"> </span></span>
<span class="line"><span style="color:#E1E4E8;">);</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">friend</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">class</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Node</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Node</span><span style="color:#24292E;"> </span></span>
<span class="line"><span style="color:#24292E;">);</span></span></code></pre></div><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>`,70),l=[o];function t(c,r,i,d,y,u){return a(),n("div",null,l)}const g=s(p,[["render",t]]);export{E as __pageData,g as default};
