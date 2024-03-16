import{_ as e,o as a,c as s,Q as n}from"./chunks/framework.0f749972.js";const E=JSON.parse('{"title":"Core::Builder","description":"","frontmatter":{"title":"Core::Builder"},"headers":[],"relativePath":"cpp-api-reference/namespaces/namespaceCore_1_1Builder.md","filePath":"cpp-api-reference/namespaces/namespaceCore_1_1Builder.md"}'),t={name:"cpp-api-reference/namespaces/namespaceCore_1_1Builder.md"},r=n(`<h1 id="core-builder" tabindex="-1">Core::Builder <a class="header-anchor" href="#core-builder" aria-label="Permalink to &quot;Core::Builder&quot;">​</a></h1><h2 id="functions" tabindex="-1">Functions <a class="header-anchor" href="#functions" aria-label="Permalink to &quot;Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td><a href="/cpp-api-reference/files/types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classcore_1_1camera.html">Camera</a> &gt;</td><td><strong><a href="/cpp-api-reference/namespaces/namespacecore_1_1builder.html#function-createcamera">createCamera</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/files/types_8h.html#using-ptr">Ptr</a>&lt; class <a href="/cpp-api-reference/classes/classcore_1_1cycle.html">Cycle</a> &gt;</td><td><strong><a href="/cpp-api-reference/namespaces/namespacecore_1_1builder.html#function-createcycle">createCycle</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/files/types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classcore_1_1model.html">Model</a> &gt;</td><td><strong><a href="/cpp-api-reference/namespaces/namespacecore_1_1builder.html#function-createmodelnode">createModelNode</a></strong>()</td></tr><tr><td>template &lt;EOperatorType T&amp;gt; &lt;br&gt;<a href="/cpp-api-reference/files/types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classcore_1_1node.html">Node</a> &gt;</td><td><strong><a href="/cpp-api-reference/namespaces/namespacecore_1_1builder.html#function-createoperator">createOperator</a></strong>()&lt;br&gt;Create new operator.</td></tr><tr><td><a href="/cpp-api-reference/files/types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classcore_1_1sequence.html">Sequence</a> &gt;</td><td><strong><a href="/cpp-api-reference/namespaces/namespacecore_1_1builder.html#function-createsequence">createSequence</a></strong>(<a href="/cpp-api-reference/classes/classcore_1_1matrixtracker.html">MatrixTracker</a> * tracker)</td></tr><tr><td>template &lt;ETransformType T&amp;gt; &lt;br&gt;<a href="/cpp-api-reference/files/types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classcore_1_1transform.html">Transform</a> &gt;</td><td><strong><a href="/cpp-api-reference/namespaces/namespacecore_1_1builder.html#function-createtransform">createTransform</a></strong>()</td></tr></tbody></table><h2 id="functions-documentation" tabindex="-1">Functions Documentation <a class="header-anchor" href="#functions-documentation" aria-label="Permalink to &quot;Functions Documentation&quot;">​</a></h2><h3 id="function-createcamera" tabindex="-1">function createCamera <a class="header-anchor" href="#function-createcamera" aria-label="Permalink to &quot;function createCamera&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Camera</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">createCamera</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Camera</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">createCamera</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-createcycle" tabindex="-1">function createCycle <a class="header-anchor" href="#function-createcycle" aria-label="Permalink to &quot;function createCycle&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#F97583;">class</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Cycle</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">createCycle</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#D73A49;">class</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Cycle</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">createCycle</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-createmodelnode" tabindex="-1">function createModelNode <a class="header-anchor" href="#function-createmodelnode" aria-label="Permalink to &quot;function createModelNode&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Model</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">createModelNode</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Model</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">createModelNode</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-createoperator" tabindex="-1">function createOperator <a class="header-anchor" href="#function-createoperator" aria-label="Permalink to &quot;function createOperator&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">EOperatorType</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">T</span><span style="color:#E1E4E8;">&gt;</span></span>
<span class="line"><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Node</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">createOperator</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">EOperatorType</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">T</span><span style="color:#24292E;">&gt;</span></span>
<span class="line"><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Node</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">createOperator</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Create new operator.</p><p><strong>Template Parameters</strong>:</p><ul><li><strong>T</strong> <a href="/cpp-api-reference/classes/structcore_1_1operation.html">Operation</a> type from OperationType enum.</li></ul><p><strong>Return</strong>: Unique pointer to newly created logic operator.</p><h3 id="function-createsequence" tabindex="-1">function createSequence <a class="header-anchor" href="#function-createsequence" aria-label="Permalink to &quot;function createSequence&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Sequence</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">createSequence</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">MatrixTracker</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">tracker</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Sequence</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">createSequence</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">MatrixTracker</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">tracker</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-createtransform" tabindex="-1">function createTransform <a class="header-anchor" href="#function-createtransform" aria-label="Permalink to &quot;function createTransform&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">ETransformType</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">T</span><span style="color:#E1E4E8;">&gt;</span></span>
<span class="line"><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Transform</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">createTransform</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">ETransformType</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">T</span><span style="color:#24292E;">&gt;</span></span>
<span class="line"><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Transform</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">createTransform</span><span style="color:#24292E;">()</span></span></code></pre></div><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>`,22),p=[r];function o(l,c,i,d,y,h){return a(),s("div",null,p)}const f=e(t,[["render",o]]);export{E as __pageData,f as default};
