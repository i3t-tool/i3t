import{_ as s,o as a,c as n,Q as e}from"./chunks/framework.0f749972.js";const u=JSON.parse('{"title":"UIModule","description":"","frontmatter":{"title":"UIModule"},"headers":[],"relativePath":"cpp-api-reference/classes/classUIModule.md","filePath":"cpp-api-reference/classes/classUIModule.md"}'),o={name:"cpp-api-reference/classes/classUIModule.md"},l=e(`<h1 id="uimodule" tabindex="-1">UIModule <a class="header-anchor" href="#uimodule" aria-label="Permalink to &quot;UIModule&quot;">​</a></h1><p><code>#include &quot;GUI/UIModule.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/classModule.html">Module</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-uimodule">UIModule</a></strong>() =default</td></tr><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-~uimodule">~UIModule</a></strong>() override</td></tr><tr><td>Fonts &amp;</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-getfonts">getFonts</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/classes/classTheme.html">Theme</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-gettheme">getTheme</a></strong>()</td></tr><tr><td>std::optional&lt; <a href="/cpp-api-reference/classes/classTheme.html">Theme</a> * &gt;</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-getthemebyname">getThemeByName</a></strong>(const std::string &amp; name) const</td></tr><tr><td>std::vector&lt; <a href="/cpp-api-reference/classes/classTheme.html">Theme</a> &gt; &amp;</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-getthemes">getThemes</a></strong>()</td></tr><tr><td><a href="/cpp-api-reference/classes/classWindowManager.html">WindowManager</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-getwindowmanager">getWindowManager</a></strong>()</td></tr><tr><td>ImFont *</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-loadfont">loadFont</a></strong>(const char * filename, float size_pixels, float fontScale, const ImFontConfig * font_cfg_template, const ImWchar * glyph_ranges, bool mergeIcons)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-loadfonts">loadFonts</a></strong>()</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-loadthemes">loadThemes</a></strong>()</td></tr><tr><td>template &lt;typename TStrategy &amp;gt; &lt;br&gt;void</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-openconfirmmodal">openConfirmModal</a></strong>()</td></tr><tr><td>template &lt;typename T &amp;gt; &lt;br&gt;void</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-openmodal">openModal</a></strong>()</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-reloadthemes">reloadThemes</a></strong>()</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-setdefaulttheme">setDefaultTheme</a></strong>()&lt;br&gt;Set the theme to the default one based on the system settings (LightMode or DarkMode only on Windows).</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#function-settheme">setTheme</a></strong>(const <a href="/cpp-api-reference/classes/classTheme.html">Theme</a> &amp; theme)</td></tr></tbody></table><h2 id="friends" tabindex="-1">Friends <a class="header-anchor" href="#friends" aria-label="Permalink to &quot;Friends&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>class</td><td><strong><a href="/cpp-api-reference/classes/classUIModule.html#friend-application">Application</a></strong></td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classModule.html">Module</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classModule.html#function-module">Module</a></strong>() =default</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classModule.html#function-~module">~Module</a></strong>() =default</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classModule.html#function-onendframe">onEndFrame</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classModule.html#function-onupdate">onUpdate</a></strong>(double deltaSeconds)</td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-uimodule" tabindex="-1">function UIModule <a class="header-anchor" href="#function-uimodule" aria-label="Permalink to &quot;function UIModule&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">UIModule</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">=default</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">UIModule</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">=default</span></span></code></pre></div><h3 id="function-uimodule-1" tabindex="-1">function ~UIModule <a class="header-anchor" href="#function-uimodule-1" aria-label="Permalink to &quot;function ~UIModule&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">~</span><span style="color:#B392F0;">UIModule</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">~</span><span style="color:#6F42C1;">UIModule</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">override</span></span></code></pre></div><h3 id="function-getfonts" tabindex="-1">function getFonts <a class="header-anchor" href="#function-getfonts" aria-label="Permalink to &quot;function getFonts&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Fonts</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getFonts</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Fonts</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getFonts</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-gettheme" tabindex="-1">function getTheme <a class="header-anchor" href="#function-gettheme" aria-label="Permalink to &quot;function getTheme&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Theme</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getTheme</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Theme</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getTheme</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getthemebyname" tabindex="-1">function getThemeByName <a class="header-anchor" href="#function-getthemebyname" aria-label="Permalink to &quot;function getThemeByName&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">optional</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Theme</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#B392F0;">getThemeByName</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">name</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">optional</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Theme</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> &gt; </span><span style="color:#6F42C1;">getThemeByName</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">name</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">const</span></span></code></pre></div><h3 id="function-getthemes" tabindex="-1">function getThemes <a class="header-anchor" href="#function-getthemes" aria-label="Permalink to &quot;function getThemes&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">vector</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Theme</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getThemes</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">vector</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Theme</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getThemes</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-getwindowmanager" tabindex="-1">function getWindowManager <a class="header-anchor" href="#function-getwindowmanager" aria-label="Permalink to &quot;function getWindowManager&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">WindowManager</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getWindowManager</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">WindowManager</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getWindowManager</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-loadfont" tabindex="-1">function loadFont <a class="header-anchor" href="#function-loadfont" aria-label="Permalink to &quot;function loadFont&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">ImFont</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">loadFont</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">char</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">filename</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">float</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">size_pixels</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">float</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">fontScale</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ImFontConfig</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">font_cfg_template</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ImWchar</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">glyph_ranges</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">mergeIcons</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">ImFont</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">loadFont</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">char</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">filename</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">float</span><span style="color:#24292E;"> </span><span style="color:#E36209;">size_pixels</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">float</span><span style="color:#24292E;"> </span><span style="color:#E36209;">fontScale</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ImFontConfig</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">font_cfg_template</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ImWchar</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">glyph_ranges</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#E36209;">mergeIcons</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-loadfonts" tabindex="-1">function loadFonts <a class="header-anchor" href="#function-loadfonts" aria-label="Permalink to &quot;function loadFonts&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">loadFonts</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">loadFonts</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-loadthemes" tabindex="-1">function loadThemes <a class="header-anchor" href="#function-loadthemes" aria-label="Permalink to &quot;function loadThemes&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">loadThemes</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">loadThemes</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-openconfirmmodal" tabindex="-1">function openConfirmModal <a class="header-anchor" href="#function-openconfirmmodal" aria-label="Permalink to &quot;function openConfirmModal&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">TStrategy</span><span style="color:#E1E4E8;"> &gt;</span></span>
<span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">openConfirmModal</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">typename</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">TStrategy</span><span style="color:#24292E;"> &gt;</span></span>
<span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">openConfirmModal</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-openmodal" tabindex="-1">function openModal <a class="header-anchor" href="#function-openmodal" aria-label="Permalink to &quot;function openModal&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">T</span><span style="color:#E1E4E8;"> &gt;</span></span>
<span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">openModal</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">typename</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">T</span><span style="color:#24292E;"> &gt;</span></span>
<span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">openModal</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-reloadthemes" tabindex="-1">function reloadThemes <a class="header-anchor" href="#function-reloadthemes" aria-label="Permalink to &quot;function reloadThemes&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">reloadThemes</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">reloadThemes</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-setdefaulttheme" tabindex="-1">function setDefaultTheme <a class="header-anchor" href="#function-setdefaulttheme" aria-label="Permalink to &quot;function setDefaultTheme&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setDefaultTheme</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setDefaultTheme</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Set the theme to the default one based on the system settings (LightMode or DarkMode only on Windows).</p><h3 id="function-settheme" tabindex="-1">function setTheme <a class="header-anchor" href="#function-settheme" aria-label="Permalink to &quot;function setTheme&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setTheme</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Theme</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">theme</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setTheme</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Theme</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">theme</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h2 id="friends-1" tabindex="-1">Friends <a class="header-anchor" href="#friends-1" aria-label="Permalink to &quot;Friends&quot;">​</a></h2><h3 id="friend-application" tabindex="-1">friend Application <a class="header-anchor" href="#friend-application" aria-label="Permalink to &quot;friend Application&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">friend</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">class</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Application</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Application</span><span style="color:#E1E4E8;"> </span></span>
<span class="line"><span style="color:#E1E4E8;">);</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">friend</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">class</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Application</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Application</span><span style="color:#24292E;"> </span></span>
<span class="line"><span style="color:#24292E;">);</span></span></code></pre></div><hr><p>Updated on 2024-10-16 at 16:58:00 +0000</p>`,47),p=[l];function t(c,r,i,d,y,h){return a(),n("div",null,p)}const m=s(o,[["render",t]]);export{u as __pageData,m as default};