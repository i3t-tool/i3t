import{_ as s,o as a,c as n,Q as l}from"./chunks/framework.0f749972.js";const u=JSON.parse('{"title":"InputController","description":"","frontmatter":{"title":"InputController"},"headers":[],"relativePath":"cpp-api-reference/classes/classInputController.md","filePath":"cpp-api-reference/classes/classInputController.md"}'),e={name:"cpp-api-reference/classes/classInputController.md"},o=l(`<h1 id="inputcontroller" tabindex="-1">InputController <a class="header-anchor" href="#inputcontroller" aria-label="Permalink to &quot;InputController&quot;">​</a></h1><p><code>#include &quot;Core/Input/InputController.h&quot;</code></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classInputController.html#function-inputcontroller">InputController</a></strong>()</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classInputController.html#function-bindaction">bindAction</a></strong>(const char * name, <a href="/cpp-api-reference/files/KeyCodes_8h.html#enum-ekeystate">EKeyState</a> state, <a href="/cpp-api-reference/files/InputController_8h.html#using-keycallback">KeyCallback</a> fn)&lt;br&gt;See <a href="./.html">InputBindings.cpp</a> file for default actions.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classInputController.html#function-bindaxis">bindAxis</a></strong>(const char * name, <a href="/cpp-api-reference/files/InputController_8h.html#using-axiscallback">AxisCallback</a> fn)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classInputController.html#function-triggeraction">triggerAction</a></strong>(const char * name, <a href="/cpp-api-reference/files/KeyCodes_8h.html#enum-ekeystate">EKeyState</a> state)&lt;br&gt;Make action active.</td></tr></tbody></table><h2 id="friends" tabindex="-1">Friends <a class="header-anchor" href="#friends" aria-label="Permalink to &quot;Friends&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>class</td><td><strong><a href="/cpp-api-reference/classes/classInputController.html#friend-inputmanager">InputManager</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-inputcontroller" tabindex="-1">function InputController <a class="header-anchor" href="#function-inputcontroller" aria-label="Permalink to &quot;function InputController&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">InputController</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">InputController</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-bindaction" tabindex="-1">function bindAction <a class="header-anchor" href="#function-bindaction" aria-label="Permalink to &quot;function bindAction&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">bindAction</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">char</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">name</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">EKeyState</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">state</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">KeyCallback</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">fn</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">bindAction</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">char</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">name</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">EKeyState</span><span style="color:#24292E;"> </span><span style="color:#E36209;">state</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">KeyCallback</span><span style="color:#24292E;"> </span><span style="color:#E36209;">fn</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>See <a href="./.html">InputBindings.cpp</a> file for default actions.</p><h3 id="function-bindaxis" tabindex="-1">function bindAxis <a class="header-anchor" href="#function-bindaxis" aria-label="Permalink to &quot;function bindAxis&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">bindAxis</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">char</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">name</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">AxisCallback</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">fn</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">bindAxis</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">char</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">name</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">AxisCallback</span><span style="color:#24292E;"> </span><span style="color:#E36209;">fn</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-triggeraction" tabindex="-1">function triggerAction <a class="header-anchor" href="#function-triggeraction" aria-label="Permalink to &quot;function triggerAction&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">triggerAction</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">char</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">name</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">EKeyState</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">state</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">triggerAction</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">char</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">name</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">EKeyState</span><span style="color:#24292E;"> </span><span style="color:#E36209;">state</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Make action active.</p><h2 id="friends-1" tabindex="-1">Friends <a class="header-anchor" href="#friends-1" aria-label="Permalink to &quot;Friends&quot;">​</a></h2><h3 id="friend-inputmanager" tabindex="-1">friend InputManager <a class="header-anchor" href="#friend-inputmanager" aria-label="Permalink to &quot;friend InputManager&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">friend</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">class</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">InputManager</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">InputManager</span><span style="color:#E1E4E8;"> </span></span>
<span class="line"><span style="color:#E1E4E8;">);</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">friend</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">class</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">InputManager</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">InputManager</span><span style="color:#24292E;"> </span></span>
<span class="line"><span style="color:#24292E;">);</span></span></code></pre></div><hr><p>Updated on 2024-10-16 at 16:44:50 +0000</p>`,22),p=[o];function t(c,r,i,d,E,y){return a(),n("div",null,p)}const f=s(e,[["render",t]]);export{u as __pageData,f as default};