import{_ as s,o as a,c as o,Q as n}from"./chunks/framework.0f749972.js";const h=JSON.parse('{"title":"Configuration","description":"","frontmatter":{"title":"Configuration"},"headers":[],"relativePath":"cpp-api-reference/classes/structConfiguration.md","filePath":"cpp-api-reference/classes/structConfiguration.md"}'),t={name:"cpp-api-reference/classes/structConfiguration.md"},l=n('<h1 id="configuration" tabindex="-1">Configuration <a class="header-anchor" href="#configuration" aria-label="Permalink to &quot;Configuration&quot;">​</a></h1><p><code>#include &quot;Config.h&quot;</code></p><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>const fs::path</td><td><strong><a href="/cpp-api-reference/classes/structConfiguration.html#variable-applog">appLog</a></strong></td></tr><tr><td>const fs::path</td><td><strong><a href="/cpp-api-reference/classes/structConfiguration.html#variable-configfile">configFile</a></strong></td></tr><tr><td>const fs::path</td><td><strong><a href="/cpp-api-reference/classes/structConfiguration.html#variable-logeventsdefinition">logEventsDefinition</a></strong></td></tr><tr><td>const fs::path</td><td><strong><a href="/cpp-api-reference/classes/structConfiguration.html#variable-mouselog">mouseLog</a></strong></td></tr><tr><td>const fs::path</td><td><strong><a href="/cpp-api-reference/classes/structConfiguration.html#variable-root">root</a></strong></td></tr><tr><td>const fs::path</td><td><strong><a href="/cpp-api-reference/classes/structConfiguration.html#variable-sceneroot">sceneRoot</a></strong></td></tr><tr><td>const fs::path</td><td><strong><a href="/cpp-api-reference/classes/structConfiguration.html#variable-userinteractionlog">userInteractionLog</a></strong></td></tr></tbody></table><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-applog" tabindex="-1">variable appLog <a class="header-anchor" href="#variable-applog" aria-label="Permalink to &quot;variable appLog&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path appLog </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> root </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Logs&quot;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;App.log&quot;</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path appLog </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> root </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Logs&quot;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;App.log&quot;</span><span style="color:#24292E;">;</span></span></code></pre></div><h3 id="variable-configfile" tabindex="-1">variable configFile <a class="header-anchor" href="#variable-configfile" aria-label="Permalink to &quot;variable configFile&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path configFile </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> root </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Data&quot;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Config.json&quot;</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path configFile </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> root </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Data&quot;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Config.json&quot;</span><span style="color:#24292E;">;</span></span></code></pre></div><h3 id="variable-logeventsdefinition" tabindex="-1">variable logEventsDefinition <a class="header-anchor" href="#variable-logeventsdefinition" aria-label="Permalink to &quot;variable logEventsDefinition&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path logEventsDefinition </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> root </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Data&quot;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;LogEventsDefinition.json&quot;</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path logEventsDefinition </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> root </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Data&quot;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;LogEventsDefinition.json&quot;</span><span style="color:#24292E;">;</span></span></code></pre></div><h3 id="variable-mouselog" tabindex="-1">variable mouseLog <a class="header-anchor" href="#variable-mouselog" aria-label="Permalink to &quot;variable mouseLog&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path mouseLog </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> root </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Logs&quot;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Mouse.log&quot;</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path mouseLog </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> root </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Logs&quot;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Mouse.log&quot;</span><span style="color:#24292E;">;</span></span></code></pre></div><h3 id="variable-root" tabindex="-1">variable root <a class="header-anchor" href="#variable-root" aria-label="Permalink to &quot;variable root&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path root </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">current_path</span><span style="color:#E1E4E8;">();</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path root </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">current_path</span><span style="color:#24292E;">();</span></span></code></pre></div><h3 id="variable-sceneroot" tabindex="-1">variable sceneRoot <a class="header-anchor" href="#variable-sceneroot" aria-label="Permalink to &quot;variable sceneRoot&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path sceneRoot </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> root </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Data&quot;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Scenes&quot;</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path sceneRoot </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> root </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Data&quot;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Scenes&quot;</span><span style="color:#24292E;">;</span></span></code></pre></div><h3 id="variable-userinteractionlog" tabindex="-1">variable userInteractionLog <a class="header-anchor" href="#variable-userinteractionlog" aria-label="Permalink to &quot;variable userInteractionLog&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path userInteractionLog </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> root </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Logs&quot;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">/</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;UserInteraction.log&quot;</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path userInteractionLog </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> root </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Logs&quot;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">/</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;UserInteraction.log&quot;</span><span style="color:#24292E;">;</span></span></code></pre></div><hr><p>Updated on 2024-10-15 at 20:41:58 +0000</p>',21),p=[l];function e(r,c,i,E,y,u){return a(),o("div",null,p)}const g=s(t,[["render",e]]);export{h as __pageData,g as default};
