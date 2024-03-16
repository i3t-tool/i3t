import{_ as s,o as a,c as n,Q as e}from"./chunks/framework.0f749972.js";const E=JSON.parse('{"title":"Application","description":"","frontmatter":{"title":"Application","summary":"Application class."},"headers":[],"relativePath":"cpp-api-reference/classes/classApplication.md","filePath":"cpp-api-reference/classes/classApplication.md"}'),o={name:"cpp-api-reference/classes/classApplication.md"},p=e(`<h1 id="application" tabindex="-1">Application <a class="header-anchor" href="#application" aria-label="Permalink to &quot;Application&quot;">​</a></h1><p><a href="./.html">Application</a> class. <a href="#detailed-description">More...</a></p><p><code>#include &quot;Core/Application.h&quot;</code></p><p>Inherited by <a href="/cpp-api-reference/classes/classi3tapplication.html">I3TApplication</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-~application">~Application</a></strong>()</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-beginframe">beginFrame</a></strong>()</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-close">close</a></strong>()&lt;br&gt;Shutdown the whole application.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-display">display</a></strong>()</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-endframe">endFrame</a></strong>()</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-enqueuecommand">enqueueCommand</a></strong>(<a href="/cpp-api-reference/classes/classicommand.html">ICommand</a> * command)&lt;br&gt;Issue command.</td></tr><tr><td>const std::string &amp;</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-gettitle">getTitle</a></strong>()</td></tr><tr><td>GLFWwindow *</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-getwindow">getWindow</a></strong>()&lt;br&gt;Get the main GLFW window.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-init">init</a></strong>()&lt;br&gt;Performs initialization of the application.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-initwindow">initWindow</a></strong>()&lt;br&gt;Init OpenGL stuffs before display loop.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-run">run</a></strong>()&lt;br&gt;Enter main loop.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-settitle">setTitle</a></strong>(const std::string &amp; title)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-update">update</a></strong>()&lt;br&gt;Perform logic update.</td></tr><tr><td>template &lt;typename T ,typename... Args&amp;gt; &lt;br&gt;T *</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-createmodule">createModule</a></strong>(Args &amp;&amp;... args)&lt;br&gt;Creates instance of module, registers it to the application, and calls its <a href="/cpp-api-reference/classes/classapplication.html#function-init">init()</a> method.</td></tr><tr><td><a href="/cpp-api-reference/classes/classapplication.html">Application</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-get">get</a></strong>()</td></tr><tr><td>GLFWwindow *</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-getcurrentwindow">getCurrentWindow</a></strong>()&lt;br&gt;Get the active GLFW window.</td></tr><tr><td>template &lt;typename T &amp;gt; &lt;br&gt;T &amp;</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-getmodule">getModule</a></strong>()</td></tr></tbody></table><h2 id="protected-functions" tabindex="-1">Protected Functions <a class="header-anchor" href="#protected-functions" aria-label="Permalink to &quot;Protected Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-application">Application</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-onbeginframe">onBeginFrame</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-onclose">onClose</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-onendframe">onEndFrame</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-oninit">onInit</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classapplication.html#function-onupdate">onUpdate</a></strong>(double delta)</td></tr></tbody></table><h2 id="detailed-description" tabindex="-1">Detailed Description <a class="header-anchor" href="#detailed-description" aria-label="Permalink to &quot;Detailed Description&quot;">​</a></h2><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">class</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Application</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">class</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Application</span><span style="color:#24292E;">;</span></span></code></pre></div><p><a href="./.html">Application</a> class.</p><p>A wrapper for <a href="/cpp-api-reference/namespaces/namespaceui.html">UI</a> windows.</p><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-application" tabindex="-1">function ~Application <a class="header-anchor" href="#function-application" aria-label="Permalink to &quot;function ~Application&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">~</span><span style="color:#B392F0;">Application</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">~</span><span style="color:#6F42C1;">Application</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-beginframe" tabindex="-1">function beginFrame <a class="header-anchor" href="#function-beginframe" aria-label="Permalink to &quot;function beginFrame&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">beginFrame</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">beginFrame</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-close" tabindex="-1">function close <a class="header-anchor" href="#function-close" aria-label="Permalink to &quot;function close&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">close</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">close</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Shutdown the whole application.</p><p><strong>See</strong>: <a href="/cpp-api-reference/classes/classapplication.html#function-onclose">onClose()</a></p><p>Called when <a href="/cpp-api-reference/classes/classclosecommand.html">CloseCommand</a> is received.</p><h3 id="function-display" tabindex="-1">function display <a class="header-anchor" href="#function-display" aria-label="Permalink to &quot;function display&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">display</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">display</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-endframe" tabindex="-1">function endFrame <a class="header-anchor" href="#function-endframe" aria-label="Permalink to &quot;function endFrame&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">endFrame</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">endFrame</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-enqueuecommand" tabindex="-1">function enqueueCommand <a class="header-anchor" href="#function-enqueuecommand" aria-label="Permalink to &quot;function enqueueCommand&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">enqueueCommand</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">ICommand</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">command</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">enqueueCommand</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">ICommand</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">command</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Issue command.</p><p><strong>Parameters</strong>:</p><ul><li><strong>command</strong> Pointer to command base class.</li></ul><p><a href="/cpp-api-reference/classes/classapplication.html">Application</a> will process all enqueued command in the main loop.</p><h3 id="function-gettitle" tabindex="-1">function getTitle <a class="header-anchor" href="#function-gettitle" aria-label="Permalink to &quot;function getTitle&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getTitle</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getTitle</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getwindow" tabindex="-1">function getWindow <a class="header-anchor" href="#function-getwindow" aria-label="Permalink to &quot;function getWindow&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">GLFWwindow</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getWindow</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">GLFWwindow</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getWindow</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Get the main GLFW window.</p><h3 id="function-init" tabindex="-1">function init <a class="header-anchor" href="#function-init" aria-label="Permalink to &quot;function init&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">init</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">init</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Performs initialization of the application.</p><p><strong>See</strong>: <a href="/cpp-api-reference/classes/classapplication.html#function-oninit">onInit()</a></p><h3 id="function-initwindow" tabindex="-1">function initWindow <a class="header-anchor" href="#function-initwindow" aria-label="Permalink to &quot;function initWindow&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">initWindow</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">initWindow</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Init OpenGL stuffs before display loop.</p><p>Taken from Muller GLFW. Initializes ImGui and <a href="/cpp-api-reference/namespaces/namespacei3t.html">I3T</a> stuffs.</p><h3 id="function-run" tabindex="-1">function run <a class="header-anchor" href="#function-run" aria-label="Permalink to &quot;function run&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">run</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">run</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Enter main loop.</p><h3 id="function-settitle" tabindex="-1">function setTitle <a class="header-anchor" href="#function-settitle" aria-label="Permalink to &quot;function setTitle&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setTitle</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">title</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setTitle</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">title</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-update" tabindex="-1">function update <a class="header-anchor" href="#function-update" aria-label="Permalink to &quot;function update&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">update</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">update</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Perform logic update.</p><p><strong>See</strong>: <a href="/cpp-api-reference/classes/classapplication.html#function-onupdate">onUpdate()</a></p><h3 id="function-createmodule" tabindex="-1">function createModule <a class="header-anchor" href="#function-createmodule" aria-label="Permalink to &quot;function createModule&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">T</span><span style="color:#E1E4E8;"> ,</span></span>
<span class="line"><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;">... </span><span style="color:#B392F0;">Args</span><span style="color:#E1E4E8;">&gt;</span></span>
<span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">T</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">createModule</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Args</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;&amp;</span><span style="color:#E1E4E8;">... </span><span style="color:#FFAB70;">args</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">typename</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">T</span><span style="color:#24292E;"> ,</span></span>
<span class="line"><span style="color:#D73A49;">typename</span><span style="color:#24292E;">... </span><span style="color:#6F42C1;">Args</span><span style="color:#24292E;">&gt;</span></span>
<span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">T</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">createModule</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Args</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;&amp;</span><span style="color:#24292E;">... </span><span style="color:#E36209;">args</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Creates instance of module, registers it to the application, and calls its <a href="/cpp-api-reference/classes/classapplication.html#function-init">init()</a> method.</p><p><strong>Parameters</strong>:</p><ul><li><strong>args</strong></li></ul><p><strong>Template Parameters</strong>:</p><ul><li><strong>T</strong></li><li><strong>Args</strong></li></ul><p><strong>Return</strong>:</p><p><strong>Note</strong>: Modules are destroyed in the reverse order of their creation!</p><h3 id="function-get" tabindex="-1">function get <a class="header-anchor" href="#function-get" aria-label="Permalink to &quot;function get&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Application</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">get</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Application</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">get</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getcurrentwindow" tabindex="-1">function getCurrentWindow <a class="header-anchor" href="#function-getcurrentwindow" aria-label="Permalink to &quot;function getCurrentWindow&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">GLFWwindow</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getCurrentWindow</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">GLFWwindow</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getCurrentWindow</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Get the active GLFW window.</p><p>This may return a different window than the <a href="/cpp-api-reference/classes/classapplication.html#function-getwindow">getWindow()</a> method due to ImGui potentially create new native windows to handle ImGui windows being dragged out of the main window.</p><h3 id="function-getmodule" tabindex="-1">function getModule <a class="header-anchor" href="#function-getmodule" aria-label="Permalink to &quot;function getModule&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">T</span><span style="color:#E1E4E8;"> &gt;</span></span>
<span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">T</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getModule</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">typename</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">T</span><span style="color:#24292E;"> &gt;</span></span>
<span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">T</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getModule</span><span style="color:#24292E;">()</span></span></code></pre></div><h2 id="protected-functions-documentation" tabindex="-1">Protected Functions Documentation <a class="header-anchor" href="#protected-functions-documentation" aria-label="Permalink to &quot;Protected Functions Documentation&quot;">​</a></h2><h3 id="function-application-1" tabindex="-1">function Application <a class="header-anchor" href="#function-application-1" aria-label="Permalink to &quot;function Application&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Application</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Application</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-onbeginframe" tabindex="-1">function onBeginFrame <a class="header-anchor" href="#function-onbeginframe" aria-label="Permalink to &quot;function onBeginFrame&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">onBeginFrame</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">onBeginFrame</span><span style="color:#24292E;">()</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classi3tapplication.html#function-onbeginframe">I3TApplication::onBeginFrame</a></p><h3 id="function-onclose" tabindex="-1">function onClose <a class="header-anchor" href="#function-onclose" aria-label="Permalink to &quot;function onClose&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">onClose</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">onClose</span><span style="color:#24292E;">()</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classi3tapplication.html#function-onclose">I3TApplication::onClose</a></p><h3 id="function-onendframe" tabindex="-1">function onEndFrame <a class="header-anchor" href="#function-onendframe" aria-label="Permalink to &quot;function onEndFrame&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">onEndFrame</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">onEndFrame</span><span style="color:#24292E;">()</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classi3tapplication.html#function-onendframe">I3TApplication::onEndFrame</a></p><h3 id="function-oninit" tabindex="-1">function onInit <a class="header-anchor" href="#function-oninit" aria-label="Permalink to &quot;function onInit&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">onInit</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">onInit</span><span style="color:#24292E;">()</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classi3tapplication.html#function-oninit">I3TApplication::onInit</a></p><h3 id="function-onupdate" tabindex="-1">function onUpdate <a class="header-anchor" href="#function-onupdate" aria-label="Permalink to &quot;function onUpdate&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">onUpdate</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">double</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">delta</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">onUpdate</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">double</span><span style="color:#24292E;"> </span><span style="color:#E36209;">delta</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classi3tapplication.html#function-onupdate">I3TApplication::onUpdate</a></p><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>`,91),t=[p];function l(c,i,r,d,u,y){return a(),n("div",null,t)}const g=s(o,[["render",l]]);export{E as __pageData,g as default};
