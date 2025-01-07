import{_ as s,o as n,c as a,Q as o}from"./chunks/framework.0f749972.js";const u=JSON.parse('{"title":"ToScriptVisitor","description":"","frontmatter":{"title":"ToScriptVisitor"},"headers":[],"relativePath":"cpp-api-reference/classes/classToScriptVisitor.md","filePath":"cpp-api-reference/classes/classToScriptVisitor.md"}'),p={name:"cpp-api-reference/classes/classToScriptVisitor.md"},t=o(`<h1 id="toscriptvisitor" tabindex="-1">ToScriptVisitor <a class="header-anchor" href="#toscriptvisitor" aria-label="Permalink to &quot;ToScriptVisitor&quot;">​</a></h1><p>Inherits from <a href="/cpp-api-reference/classes/classNodeVisitor.html">NodeVisitor</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classToScriptVisitor.html#function-append">append</a></strong>(const std::string &amp; str)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classToScriptVisitor.html#function-appendconnections">appendConnections</a></strong>(const <a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classWorkspace_1_1CoreNode.html">Workspace::CoreNode</a> &gt; &amp; node)</td></tr><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/classToScriptVisitor.html#function-getscript">getScript</a></strong>() const</td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-append" tabindex="-1">function append <a class="header-anchor" href="#function-append" aria-label="Permalink to &quot;function append&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">append</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">str</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">append</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">str</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-appendconnections" tabindex="-1">function appendConnections <a class="header-anchor" href="#function-appendconnections" aria-label="Permalink to &quot;function appendConnections&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">appendConnections</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Workspace</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">CoreNode</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">node</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">appendConnections</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Workspace</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">CoreNode</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">node</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-getscript" tabindex="-1">function getScript <a class="header-anchor" href="#function-getscript" aria-label="Permalink to &quot;function getScript&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getScript</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getScript</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><hr><p>Updated on 2025-01-07 at 09:46:55 +0000</p>`,13),e=[t];function l(c,r,i,d,y,E){return n(),a("div",null,e)}const f=s(p,[["render",l]]);export{u as __pageData,f as default};
