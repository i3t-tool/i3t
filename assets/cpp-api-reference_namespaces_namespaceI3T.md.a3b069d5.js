import{_ as s,o as a,c as e,Q as n}from"./chunks/framework.0f749972.js";const g=JSON.parse('{"title":"I3T","description":"","frontmatter":{"title":"I3T"},"headers":[],"relativePath":"cpp-api-reference/namespaces/namespaceI3T.md","filePath":"cpp-api-reference/namespaces/namespaceI3T.md"}'),p={name:"cpp-api-reference/namespaces/namespaceI3T.md"},o=n(`<h1 id="i3t" tabindex="-1">I3T <a class="header-anchor" href="#i3t" aria-label="Permalink to &quot;I3T&quot;">​</a></h1><h2 id="functions" tabindex="-1">Functions <a class="header-anchor" href="#functions" aria-label="Permalink to &quot;Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td><a href="/cpp-api-reference/classes/classI3TApplication.html">I3TApplication</a> &amp;</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-app">app</a></strong>()</td></tr><tr><td>template &lt;typename Theme_ &amp;gt; &lt;br&gt;void</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-emplacetheme">emplaceTheme</a></strong>(Theme_ &amp;&amp; theme)&lt;br&gt;Change properties of the given theme.</td></tr><tr><td>const ImVec4 &amp;</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-getcolor">getColor</a></strong>(<a href="/cpp-api-reference/files/Theme_8h.html#enum-ecolor">EColor</a> color)</td></tr><tr><td>ImFont *</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-getfont">getFont</a></strong>(<a href="/cpp-api-reference/files/Theme_8h.html#enum-efont">EFont</a> font)</td></tr><tr><td><a href="/cpp-api-reference/classes/classCore_1_1ResourceManager.html">Core::ResourceManager</a> &amp;</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-getresourcemanager">getResourceManager</a></strong>()</td></tr><tr><td>float</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-getsize">getSize</a></strong>(<a href="/cpp-api-reference/files/Theme_8h.html#enum-esize">ESize</a> size)</td></tr><tr><td>const ImVec2 &amp;</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-getsize">getSize</a></strong>(<a href="/cpp-api-reference/files/Theme_8h.html#enum-esizevec2">ESizeVec2</a> size)</td></tr><tr><td><a href="/cpp-api-reference/classes/classTheme.html">Theme</a> &amp;</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-gettheme">getTheme</a></strong>()</td></tr><tr><td>std::vector&lt; <a href="/cpp-api-reference/classes/classTheme.html">Theme</a> &gt; &amp;</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-getthemes">getThemes</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/classes/classUIModule.html">UIModule</a> *</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-getui">getUI</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/classes/classVp_1_1Viewport.html">Vp::Viewport</a> *</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-getviewport">getViewport</a></strong>()</td></tr><tr><td>template &lt;typename T &amp;gt; &lt;br&gt;<a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; T &gt;</td><td><strong><a href="/cpp-api-reference/namespaces/namespaceI3T.html#function-getwindowptr">getWindowPtr</a></strong>()&lt;br&gt;Get pointer to dockable (unique) window.</td></tr></tbody></table><h2 id="functions-documentation" tabindex="-1">Functions Documentation <a class="header-anchor" href="#functions-documentation" aria-label="Permalink to &quot;Functions Documentation&quot;">​</a></h2><h3 id="function-app" tabindex="-1">function app <a class="header-anchor" href="#function-app" aria-label="Permalink to &quot;function app&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">I3TApplication</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">app</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">I3TApplication</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">app</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-emplacetheme" tabindex="-1">function emplaceTheme <a class="header-anchor" href="#function-emplacetheme" aria-label="Permalink to &quot;function emplaceTheme&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Theme_</span><span style="color:#E1E4E8;"> &gt;</span></span>
<span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">emplaceTheme</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Theme_</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">theme</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">typename</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Theme_</span><span style="color:#24292E;"> &gt;</span></span>
<span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">emplaceTheme</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Theme_</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">theme</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Change properties of the given theme.</p><p><strong>Parameters</strong>:</p><ul><li><strong>theme</strong></li></ul><h3 id="function-getcolor" tabindex="-1">function getColor <a class="header-anchor" href="#function-getcolor" aria-label="Permalink to &quot;function getColor&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ImVec4</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getColor</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">EColor</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">color</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ImVec4</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getColor</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">EColor</span><span style="color:#24292E;"> </span><span style="color:#E36209;">color</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-getfont" tabindex="-1">function getFont <a class="header-anchor" href="#function-getfont" aria-label="Permalink to &quot;function getFont&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">ImFont</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getFont</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">EFont</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">font</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">ImFont</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getFont</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">EFont</span><span style="color:#24292E;"> </span><span style="color:#E36209;">font</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-getresourcemanager" tabindex="-1">function getResourceManager <a class="header-anchor" href="#function-getresourcemanager" aria-label="Permalink to &quot;function getResourceManager&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Core</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">ResourceManager</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getResourceManager</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Core</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">ResourceManager</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getResourceManager</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getsize" tabindex="-1">function getSize <a class="header-anchor" href="#function-getsize" aria-label="Permalink to &quot;function getSize&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">float</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getSize</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">ESize</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">size</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">float</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getSize</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">ESize</span><span style="color:#24292E;"> </span><span style="color:#E36209;">size</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-getsize-1" tabindex="-1">function getSize <a class="header-anchor" href="#function-getsize-1" aria-label="Permalink to &quot;function getSize&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ImVec2</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getSize</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">ESizeVec2</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">size</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ImVec2</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getSize</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">ESizeVec2</span><span style="color:#24292E;"> </span><span style="color:#E36209;">size</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-gettheme" tabindex="-1">function getTheme <a class="header-anchor" href="#function-gettheme" aria-label="Permalink to &quot;function getTheme&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Theme</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getTheme</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Theme</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getTheme</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getthemes" tabindex="-1">function getThemes <a class="header-anchor" href="#function-getthemes" aria-label="Permalink to &quot;function getThemes&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">vector</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Theme</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getThemes</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">vector</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Theme</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getThemes</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getui" tabindex="-1">function getUI <a class="header-anchor" href="#function-getui" aria-label="Permalink to &quot;function getUI&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">UIModule</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getUI</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">UIModule</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getUI</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getviewport" tabindex="-1">function getViewport <a class="header-anchor" href="#function-getviewport" aria-label="Permalink to &quot;function getViewport&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Vp</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">Viewport</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getViewport</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Vp</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">Viewport</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getViewport</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getwindowptr" tabindex="-1">function getWindowPtr <a class="header-anchor" href="#function-getwindowptr" aria-label="Permalink to &quot;function getWindowPtr&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">T</span><span style="color:#E1E4E8;"> &gt;</span></span>
<span class="line"><span style="color:#B392F0;">Ptr</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">T</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">getWindowPtr</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">typename</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">T</span><span style="color:#24292E;"> &gt;</span></span>
<span class="line"><span style="color:#6F42C1;">Ptr</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">T</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">getWindowPtr</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Get pointer to dockable (unique) window.</p><p><strong>Template Parameters</strong>:</p><ul><li><strong>T</strong> window type</li></ul><p><strong>Return</strong>: shared pointer to window of type T.</p><hr><p>Updated on 2024-10-16 at 16:44:50 +0000</p>`,37),t=[o];function l(c,r,i,d,y,h){return a(),e("div",null,t)}const m=s(p,[["render",l]]);export{g as __pageData,m as default};