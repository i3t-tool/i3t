import{_ as s,o as a,c as e,Q as t}from"./chunks/framework.0f749972.js";const b=JSON.parse('{"title":"UserData","description":"","frontmatter":{"title":"UserData","summary":"This struct is reflected by RTTR."},"headers":[],"relativePath":"cpp-api-reference/classes/structUserData.md","filePath":"cpp-api-reference/classes/structUserData.md"}'),n={name:"cpp-api-reference/classes/structUserData.md"},l=t(`<h1 id="userdata" tabindex="-1">UserData <a class="header-anchor" href="#userdata" aria-label="Permalink to &quot;UserData&quot;">​</a></h1><p>This struct is reflected by RTTR. <a href="#detailed-description">More...</a></p><p><code>#include &quot;UserData.h&quot;</code></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/structUserData.html#function-pushrecentfile">pushRecentFile</a></strong>(const std::filesystem::path &amp; file)</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/structUserData.html#variable-customthemename">customThemeName</a></strong> &lt;br&gt;If empty, LightMode or DarkMode is used based on system settings.</td></tr><tr><td>std::vector&lt; std::filesystem::path &gt;</td><td><strong><a href="/cpp-api-reference/classes/structUserData.html#variable-recentfiles">recentFiles</a></strong> &lt;br&gt;Last file is the most recent.</td></tr></tbody></table><h2 id="detailed-description" tabindex="-1">Detailed Description <a class="header-anchor" href="#detailed-description" aria-label="Permalink to &quot;Detailed Description&quot;">​</a></h2><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">struct</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">UserData</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">struct</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">UserData</span><span style="color:#24292E;">;</span></span></code></pre></div><p>This struct is reflected by RTTR.</p><p><strong>See</strong>: <a href="/cpp-api-reference/files/UserData_8cpp#file-userdata.cpp">UserData.cpp</a> file.</p><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-pushrecentfile" tabindex="-1">function pushRecentFile <a class="header-anchor" href="#function-pushrecentfile" aria-label="Permalink to &quot;function pushRecentFile&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">pushRecentFile</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">filesystem</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">path</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">file</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">pushRecentFile</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">filesystem</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">path</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">file</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-customthemename" tabindex="-1">variable customThemeName <a class="header-anchor" href="#variable-customthemename" aria-label="Permalink to &quot;variable customThemeName&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::string customThemeName;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::string customThemeName;</span></span></code></pre></div><p>If empty, LightMode or DarkMode is used based on system settings.</p><h3 id="variable-recentfiles" tabindex="-1">variable recentFiles <a class="header-anchor" href="#variable-recentfiles" aria-label="Permalink to &quot;variable recentFiles&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::vector</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">filesystem</span><span style="color:#E1E4E8;">::path </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> recentFiles;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::vector</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">filesystem</span><span style="color:#24292E;">::path </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> recentFiles;</span></span></code></pre></div><p>Last file is the most recent.</p><hr><p>Updated on 2024-10-16 at 16:58:00 +0000</p>`,23),o=[l];function p(c,r,i,d,h,u){return a(),e("div",null,o)}const m=s(n,[["render",p]]);export{b as __pageData,m as default};