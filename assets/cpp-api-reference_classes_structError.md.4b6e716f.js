import{_ as s,o as a,c as n,Q as o}from"./chunks/framework.0f749972.js";const y=JSON.parse('{"title":"Error","description":"","frontmatter":{"title":"Error"},"headers":[],"relativePath":"cpp-api-reference/classes/structError.md","filePath":"cpp-api-reference/classes/structError.md"}'),r={name:"cpp-api-reference/classes/structError.md"},t=o(`<h1 id="error" tabindex="-1">Error <a class="header-anchor" href="#error" aria-label="Permalink to &quot;Error&quot;">​</a></h1><p><code>#include &quot;Core/Result.h&quot;</code></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/structError.html#function-error">Error</a></strong>() =default</td></tr><tr><td></td><td><strong><a href="/cpp-api-reference/classes/structError.html#function-error">Error</a></strong>(const char * message)</td></tr><tr><td></td><td><strong><a href="/cpp-api-reference/classes/structError.html#function-error">Error</a></strong>(std::string message)</td></tr><tr><td>const char *</td><td><strong><a href="/cpp-api-reference/classes/structError.html#function-str">str</a></strong>() const</td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-error" tabindex="-1">function Error <a class="header-anchor" href="#function-error" aria-label="Permalink to &quot;function Error&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Error</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">=default</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Error</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">=default</span></span></code></pre></div><h3 id="function-error-1" tabindex="-1">function Error <a class="header-anchor" href="#function-error-1" aria-label="Permalink to &quot;function Error&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Error</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">char</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> message</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Error</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">char</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> message</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-error-2" tabindex="-1">function Error <a class="header-anchor" href="#function-error-2" aria-label="Permalink to &quot;function Error&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Error</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::string message</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Error</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::string message</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-str" tabindex="-1">function str <a class="header-anchor" href="#function-str" aria-label="Permalink to &quot;function str&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">char</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">str</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">char</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">str</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><hr><p>Updated on 2024-10-16 at 16:58:00 +0000</p>`,15),e=[t];function l(p,c,i,d,E,u){return a(),n("div",null,e)}const f=s(r,[["render",l]]);export{y as __pageData,f as default};