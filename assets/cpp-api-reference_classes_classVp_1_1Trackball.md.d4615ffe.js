import{_ as s,o as a,c as n,Q as l}from"./chunks/framework.0f749972.js";const u=JSON.parse('{"title":"Vp::Trackball","description":"","frontmatter":{"title":"Vp::Trackball","summary":"Static utility class for trackball interaction."},"headers":[],"relativePath":"cpp-api-reference/classes/classVp_1_1Trackball.md","filePath":"cpp-api-reference/classes/classVp_1_1Trackball.md"}'),e={name:"cpp-api-reference/classes/classVp_1_1Trackball.md"},t=l(`<h1 id="vp-trackball" tabindex="-1">Vp::Trackball <a class="header-anchor" href="#vp-trackball" aria-label="Permalink to &quot;Vp::Trackball&quot;">​</a></h1><p>Static utility class for trackball interaction.</p><p><code>#include &quot;Viewport/camera/Trackball.h&quot;</code></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>glm::mat4</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1trackball.html#function-rotate">rotate</a></strong>(glm::vec2 screenStart, glm::vec2 screenEnd, glm::ivec2 screenSize =glm::ivec2(1, 1))&lt;br&gt;Resolves trackball movement from one point to another and returns a corresponding rotation matrix.</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>glm::vec2</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1trackball.html#variable-debug-trackballscreensize">debug_trackballScreenSize</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-rotate" tabindex="-1">function rotate <a class="header-anchor" href="#function-rotate" aria-label="Permalink to &quot;function rotate&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">mat4</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">rotate</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">vec2</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">screenStart</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">vec2</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">screenEnd</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">ivec2</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">screenSize</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">=</span><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">ivec2</span><span style="color:#E1E4E8;">(</span><span style="color:#79B8FF;">1</span><span style="color:#E1E4E8;">, </span><span style="color:#79B8FF;">1</span><span style="color:#E1E4E8;">)</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">mat4</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">rotate</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">vec2</span><span style="color:#24292E;"> </span><span style="color:#E36209;">screenStart</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">vec2</span><span style="color:#24292E;"> </span><span style="color:#E36209;">screenEnd</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">ivec2</span><span style="color:#24292E;"> </span><span style="color:#E36209;">screenSize</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">=</span><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">ivec2</span><span style="color:#24292E;">(</span><span style="color:#005CC5;">1</span><span style="color:#24292E;">, </span><span style="color:#005CC5;">1</span><span style="color:#24292E;">)</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Resolves trackball movement from one point to another and returns a corresponding rotation matrix.</p><p><strong>Parameters</strong>:</p><ul><li><strong>screenStart</strong> Start point in range (-1, 1) or (0, screenSize)</li><li><strong>screenEnd</strong> End point in range (-1, 1) or (0, screenSize)</li><li><strong>screenSize</strong> Optionally screen size</li></ul><p><strong>Return</strong>: Immediate rotation matrix</p><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-debug-trackballscreensize" tabindex="-1">variable debug_trackballScreenSize <a class="header-anchor" href="#variable-debug-trackballscreensize" aria-label="Permalink to &quot;variable debug_trackballScreenSize&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::vec2 debug_trackballScreenSize </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">vec2</span><span style="color:#E1E4E8;">(</span><span style="color:#79B8FF;">0</span><span style="color:#E1E4E8;">);</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::vec2 debug_trackballScreenSize </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">vec2</span><span style="color:#24292E;">(</span><span style="color:#005CC5;">0</span><span style="color:#24292E;">);</span></span></code></pre></div><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>`,19),o=[t];function p(c,r,i,E,y,d){return a(),n("div",null,o)}const h=s(e,[["render",p]]);export{u as __pageData,h as default};
