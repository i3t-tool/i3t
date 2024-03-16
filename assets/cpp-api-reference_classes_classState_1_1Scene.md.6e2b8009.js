import{_ as a,o as e,c as s,Q as t}from"./chunks/framework.0f749972.js";const v=JSON.parse('{"title":"State::Scene","description":"","frontmatter":{"title":"State::Scene"},"headers":[],"relativePath":"cpp-api-reference/classes/classState_1_1Scene.md","filePath":"cpp-api-reference/classes/classState_1_1Scene.md"}'),n={name:"cpp-api-reference/classes/classState_1_1Scene.md"},l=t(`<h1 id="state-scene" tabindex="-1">State::Scene <a class="header-anchor" href="#state-scene" aria-label="Permalink to &quot;State::Scene&quot;">​</a></h1><p><code>#include &quot;State/Scene.h&quot;</code></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#function-scene">Scene</a></strong>(<a href="/cpp-api-reference/classes/classstatemanager.html">StateManager</a> * stateManager, bool readOnly)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#function-issaved">isSaved</a></strong>() const</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>fs::path</td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#variable-m-datapath">m_dataPath</a></strong> &lt;br&gt;Path to the scene data directory.</td></tr><tr><td>long</td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#variable-m-hash">m_hash</a></strong></td></tr><tr><td>fs::path</td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#variable-m-path">m_path</a></strong> &lt;br&gt;Path to the scene json file.</td></tr><tr><td>fs::path</td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#variable-m-prevdatapath">m_prevDataPath</a></strong> &lt;br&gt;m_dataPath of the previous saved scene, valid if m_sceneLocationChanged is true</td></tr><tr><td>fs::path</td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#variable-m-prevpath">m_prevPath</a></strong> &lt;br&gt;m_path of the previous saved scene, valid if m_sceneLocationChanged is true</td></tr><tr><td>const bool</td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#variable-m-readonly">m_readOnly</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#variable-m-scenelocationchanged">m_sceneLocationChanged</a></strong> &lt;br&gt;Whether the scene path is a new location compared to the previous save.</td></tr><tr><td><a href="/cpp-api-reference/classes/classstatemanager.html">StateManager</a> *</td><td><strong><a href="/cpp-api-reference/classes/classstate_1_1scene.html#variable-m-statemanager">m_stateManager</a></strong> &lt;br&gt;Reference to the state manager this scene is managed by.</td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-scene" tabindex="-1">function Scene <a class="header-anchor" href="#function-scene" aria-label="Permalink to &quot;function Scene&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Scene</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    StateManager </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> stateManager,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> readOnly</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Scene</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    StateManager </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> stateManager,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> readOnly</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-issaved" tabindex="-1">function isSaved <a class="header-anchor" href="#function-issaved" aria-label="Permalink to &quot;function isSaved&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">isSaved</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">isSaved</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-datapath" tabindex="-1">variable m_dataPath <a class="header-anchor" href="#variable-m-datapath" aria-label="Permalink to &quot;variable m_dataPath&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path m_dataPath;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path m_dataPath;</span></span></code></pre></div><p>Path to the scene data directory.</p><h3 id="variable-m-hash" tabindex="-1">variable m_hash <a class="header-anchor" href="#variable-m-hash" aria-label="Permalink to &quot;variable m_hash&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">long</span><span style="color:#E1E4E8;"> m_hash {};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">long</span><span style="color:#24292E;"> m_hash {};</span></span></code></pre></div><h3 id="variable-m-path" tabindex="-1">variable m_path <a class="header-anchor" href="#variable-m-path" aria-label="Permalink to &quot;variable m_path&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path m_path;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path m_path;</span></span></code></pre></div><p>Path to the scene json file.</p><h3 id="variable-m-prevdatapath" tabindex="-1">variable m_prevDataPath <a class="header-anchor" href="#variable-m-prevdatapath" aria-label="Permalink to &quot;variable m_prevDataPath&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path m_prevDataPath;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path m_prevDataPath;</span></span></code></pre></div><p>m_dataPath of the previous saved scene, valid if m_sceneLocationChanged is true</p><h3 id="variable-m-prevpath" tabindex="-1">variable m_prevPath <a class="header-anchor" href="#variable-m-prevpath" aria-label="Permalink to &quot;variable m_prevPath&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path m_prevPath;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path m_prevPath;</span></span></code></pre></div><p>m_path of the previous saved scene, valid if m_sceneLocationChanged is true</p><h3 id="variable-m-readonly" tabindex="-1">variable m_readOnly <a class="header-anchor" href="#variable-m-readonly" aria-label="Permalink to &quot;variable m_readOnly&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> m_readOnly;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> m_readOnly;</span></span></code></pre></div><h3 id="variable-m-scenelocationchanged" tabindex="-1">variable m_sceneLocationChanged <a class="header-anchor" href="#variable-m-scenelocationchanged" aria-label="Permalink to &quot;variable m_sceneLocationChanged&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> m_sceneLocationChanged {</span><span style="color:#79B8FF;">false</span><span style="color:#E1E4E8;">};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> m_sceneLocationChanged {</span><span style="color:#005CC5;">false</span><span style="color:#24292E;">};</span></span></code></pre></div><p>Whether the scene path is a new location compared to the previous save.</p><h3 id="variable-m-statemanager" tabindex="-1">variable m_stateManager <a class="header-anchor" href="#variable-m-statemanager" aria-label="Permalink to &quot;variable m_stateManager&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">StateManager </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> m_stateManager;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">StateManager </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> m_stateManager;</span></span></code></pre></div><p>Reference to the state manager this scene is managed by.</p><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>`,36),p=[l];function o(c,r,i,d,h,b){return e(),s("div",null,p)}const m=a(n,[["render",o]]);export{v as __pageData,m as default};
