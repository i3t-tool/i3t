import{_ as s,o as a,c as n,Q as e}from"./chunks/framework.0f749972.js";const u=JSON.parse('{"title":"Chronos","description":"","frontmatter":{"title":"Chronos"},"headers":[],"relativePath":"cpp-api-reference/classes/classChronos.md","filePath":"cpp-api-reference/classes/classChronos.md"}'),l={name:"cpp-api-reference/classes/classChronos.md"},o=e(`<h1 id="chronos" tabindex="-1">Chronos <a class="header-anchor" href="#chronos" aria-label="Permalink to &quot;Chronos&quot;">​</a></h1><p><code>#include &quot;Scripting/Timer.h&quot;</code></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classChronos.html#function-cleartimer">clearTimer</a></strong>(<a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classTimer.html">Timer</a> &gt; timer)</td></tr><tr><td><a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classTimer.html">Timer</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classChronos.html#function-settimer">setTimer</a></strong>(uint64_t intervalMs, sol::protected_function callback)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classChronos.html#function-update">update</a></strong>(double deltaSeconds)</td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-cleartimer" tabindex="-1">function clearTimer <a class="header-anchor" href="#function-cleartimer" aria-label="Permalink to &quot;function clearTimer&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">clearTimer</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Timer</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#FFAB70;">timer</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">clearTimer</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Timer</span><span style="color:#24292E;"> &gt; </span><span style="color:#E36209;">timer</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-settimer" tabindex="-1">function setTimer <a class="header-anchor" href="#function-settimer" aria-label="Permalink to &quot;function setTimer&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Timer</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">setTimer</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">uint64_t</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">intervalMs</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">sol</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">protected_function</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">callback</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Timer</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">setTimer</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">uint64_t</span><span style="color:#24292E;"> </span><span style="color:#E36209;">intervalMs</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">sol</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">protected_function</span><span style="color:#24292E;"> </span><span style="color:#E36209;">callback</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-update" tabindex="-1">function update <a class="header-anchor" href="#function-update" aria-label="Permalink to &quot;function update&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">update</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">double</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">deltaSeconds</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">update</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">double</span><span style="color:#24292E;"> </span><span style="color:#E36209;">deltaSeconds</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><hr><p>Updated on 2024-10-15 at 20:41:58 +0000</p>`,13),p=[o];function t(c,r,i,d,E,y){return a(),n("div",null,p)}const m=s(l,[["render",t]]);export{u as __pageData,m as default};
