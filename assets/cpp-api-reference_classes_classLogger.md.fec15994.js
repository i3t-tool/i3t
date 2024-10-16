import{_ as s,o as a,c as n,Q as o}from"./chunks/framework.0f749972.js";const h=JSON.parse('{"title":"Logger","description":"","frontmatter":{"title":"Logger"},"headers":[],"relativePath":"cpp-api-reference/classes/classLogger.md","filePath":"cpp-api-reference/classes/classLogger.md"}'),p={name:"cpp-api-reference/classes/classLogger.md"},l=o(`<h1 id="logger" tabindex="-1">Logger <a class="header-anchor" href="#logger" aria-label="Permalink to &quot;Logger&quot;">​</a></h1><p><code>#include &quot;Logger/Logger.h&quot;</code></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-logger">Logger</a></strong>(const <a href="/cpp-api-reference/classes/classLogger.html">Logger</a> &amp; ) =delete</td></tr><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-logger">Logger</a></strong>(<a href="/cpp-api-reference/classes/classLogger.html">Logger</a> &amp;&amp; ) =delete</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-addtologbuffer">addToLogBuffer</a></strong>(const <a href="/cpp-api-reference/files/Logger_8h.html#enum-loggingoption">LoggingOption</a> &amp; logType, const std::string &amp; message, const std::vector&lt; std::string &gt; &amp; args)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-endlogger">endLogger</a></strong>() const</td></tr><tr><td>std::shared_ptr&lt; spdlog::logger &gt;</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-getapplogger">getAppLogger</a></strong>()</td></tr><tr><td>std::ostringstream &amp;</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-getbuffer">getBuffer</a></strong>()</td></tr><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-getlogstring">getLogString</a></strong>(const std::string &amp; key) const</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-initlogger">initLogger</a></strong>(int argc, char * argv[])</td></tr><tr><td>template &lt;typename... Args&amp;gt; &lt;br&gt;void</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-log">log</a></strong>(const <a href="/cpp-api-reference/files/Logger_8h.html#enum-loggingoption">LoggingOption</a> &amp; logType, const std::string &amp; message, Args &amp;&amp;... args)</td></tr><tr><td><a href="/cpp-api-reference/classes/classLogger.html">Logger</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-operator=">operator=</a></strong>(const <a href="/cpp-api-reference/classes/classLogger.html">Logger</a> &amp; ) =delete</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-update">update</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/classes/classLogger.html">Logger</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classLogger.html#function-getinstance">getInstance</a></strong>()&lt;br&gt;<a href="/cpp-api-reference/classes/classLogger.html">Logger</a> class is a singleton.</td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-logger" tabindex="-1">function Logger <a class="header-anchor" href="#function-logger" aria-label="Permalink to &quot;function Logger&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Logger</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> Logger </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=delete</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Logger</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> Logger </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=delete</span></span></code></pre></div><h3 id="function-logger-1" tabindex="-1">function Logger <a class="header-anchor" href="#function-logger-1" aria-label="Permalink to &quot;function Logger&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Logger</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    Logger </span><span style="color:#F97583;">&amp;&amp;</span><span style="color:#E1E4E8;"> </span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=delete</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Logger</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    Logger </span><span style="color:#D73A49;">&amp;&amp;</span><span style="color:#24292E;"> </span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=delete</span></span></code></pre></div><h3 id="function-addtologbuffer" tabindex="-1">function addToLogBuffer <a class="header-anchor" href="#function-addtologbuffer" aria-label="Permalink to &quot;function addToLogBuffer&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">addToLogBuffer</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">LoggingOption</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">logType</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">message</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">vector</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">args</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">addToLogBuffer</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">LoggingOption</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">logType</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">message</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">vector</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">args</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-endlogger" tabindex="-1">function endLogger <a class="header-anchor" href="#function-endlogger" aria-label="Permalink to &quot;function endLogger&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">endLogger</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">endLogger</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><h3 id="function-getapplogger" tabindex="-1">function getAppLogger <a class="header-anchor" href="#function-getapplogger" aria-label="Permalink to &quot;function getAppLogger&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">shared_ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">spdlog</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">logger</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">getAppLogger</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">shared_ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">spdlog</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">logger</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">getAppLogger</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getbuffer" tabindex="-1">function getBuffer <a class="header-anchor" href="#function-getbuffer" aria-label="Permalink to &quot;function getBuffer&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">ostringstream</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getBuffer</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">ostringstream</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getBuffer</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getlogstring" tabindex="-1">function getLogString <a class="header-anchor" href="#function-getlogstring" aria-label="Permalink to &quot;function getLogString&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getLogString</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">key</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getLogString</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">key</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">const</span></span></code></pre></div><h3 id="function-initlogger" tabindex="-1">function initLogger <a class="header-anchor" href="#function-initlogger" aria-label="Permalink to &quot;function initLogger&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">initLogger</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">int</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">argc</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">char</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">argv</span><span style="color:#E1E4E8;">[]</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">initLogger</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">int</span><span style="color:#24292E;"> </span><span style="color:#E36209;">argc</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">char</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">argv</span><span style="color:#24292E;">[]</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-log" tabindex="-1">function log <a class="header-anchor" href="#function-log" aria-label="Permalink to &quot;function log&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;">... </span><span style="color:#B392F0;">Args</span><span style="color:#E1E4E8;">&gt;</span></span>
<span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">log</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">LoggingOption</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">logType</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">message</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Args</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;&amp;</span><span style="color:#E1E4E8;">... </span><span style="color:#FFAB70;">args</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">typename</span><span style="color:#24292E;">... </span><span style="color:#6F42C1;">Args</span><span style="color:#24292E;">&gt;</span></span>
<span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">log</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">LoggingOption</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">logType</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">message</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Args</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;&amp;</span><span style="color:#24292E;">... </span><span style="color:#E36209;">args</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-operator" tabindex="-1">function operator= <a class="header-anchor" href="#function-operator" aria-label="Permalink to &quot;function operator=&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Logger</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">operator</span><span style="color:#B392F0;">=</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Logger</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=delete</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Logger</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">operator</span><span style="color:#6F42C1;">=</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Logger</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=delete</span></span></code></pre></div><h3 id="function-update" tabindex="-1">function update <a class="header-anchor" href="#function-update" aria-label="Permalink to &quot;function update&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">update</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">update</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getinstance" tabindex="-1">function getInstance <a class="header-anchor" href="#function-getinstance" aria-label="Permalink to &quot;function getInstance&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Logger</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getInstance</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Logger</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getInstance</span><span style="color:#24292E;">()</span></span></code></pre></div><p><a href="/cpp-api-reference/classes/classLogger.html">Logger</a> class is a singleton.</p><p><strong>Return</strong>: <a href="/cpp-api-reference/classes/classLogger.html">Logger</a> instance.</p><hr><p>Updated on 2024-10-16 at 16:44:50 +0000</p>`,33),e=[l];function t(c,r,i,g,y,E){return a(),n("div",null,e)}const u=s(p,[["render",t]]);export{h as __pageData,u as default};