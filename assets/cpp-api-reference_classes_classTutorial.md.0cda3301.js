import{_ as s,o as a,c as t,Q as e}from"./chunks/framework.0f749972.js";const E=JSON.parse('{"title":"Tutorial","description":"","frontmatter":{"title":"Tutorial"},"headers":[],"relativePath":"cpp-api-reference/classes/classTutorial.md","filePath":"cpp-api-reference/classes/classTutorial.md"}'),l={name:"cpp-api-reference/classes/classTutorial.md"},n=e(`<h1 id="tutorial" tabindex="-1">Tutorial <a class="header-anchor" href="#tutorial" aria-label="Permalink to &quot;Tutorial&quot;">​</a></h1><p><code>#include &quot;Tutorial/Tutorial.h&quot;</code></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classTutorial.html#function-tutorial">Tutorial</a></strong>(std::shared_ptr&lt; <a href="/cpp-api-reference/classes/structTutorialHeader.html">TutorialHeader</a> &gt; header, std::vector&lt; <a href="/cpp-api-reference/classes/structTutorialStep.html">TutorialStep</a> &gt; steps, std::unordered_map&lt; std::string, std::shared_ptr&lt; <a href="/cpp-api-reference/classes/structGUIImage.html">GUIImage</a> &gt; &gt; filenameToImageMap)</td></tr><tr><td>auto</td><td><strong><a href="/cpp-api-reference/classes/classTutorial.html#function-getstepcount">getStepCount</a></strong>() const</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>std::unordered_map&lt; std::string, std::shared_ptr&lt; <a href="/cpp-api-reference/classes/structGUIImage.html">GUIImage</a> &gt; &gt;</td><td><strong><a href="/cpp-api-reference/classes/classTutorial.html#variable-m-filenametoimage">m_filenameToImage</a></strong> &lt;br&gt;support structures, filename to <a href="/cpp-api-reference/classes/structGUIImage.html">GUIImage</a> (including GLuint id)</td></tr><tr><td>std::shared_ptr&lt; <a href="/cpp-api-reference/classes/structTutorialHeader.html">TutorialHeader</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classTutorial.html#variable-m-header">m_header</a></strong></td></tr><tr><td>std::vector&lt; <a href="/cpp-api-reference/classes/structTutorialStep.html">TutorialStep</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classTutorial.html#variable-m-steps">m_steps</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-tutorial" tabindex="-1">function Tutorial <a class="header-anchor" href="#function-tutorial" aria-label="Permalink to &quot;function Tutorial&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Tutorial</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::shared_ptr</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> TutorialHeader </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> header,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::vector</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> TutorialStep </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> steps,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::unordered_map</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::string, </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::shared_ptr</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> GUIImage </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> filenameToImageMap</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Tutorial</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::shared_ptr</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> TutorialHeader </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> header,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::vector</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> TutorialStep </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> steps,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::unordered_map</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::string, </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::shared_ptr</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> GUIImage </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> filenameToImageMap</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-getstepcount" tabindex="-1">function getStepCount <a class="header-anchor" href="#function-getstepcount" aria-label="Permalink to &quot;function getStepCount&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">auto</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">getStepCount</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">auto</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">getStepCount</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-filenametoimage" tabindex="-1">variable m_filenameToImage <a class="header-anchor" href="#variable-m-filenametoimage" aria-label="Permalink to &quot;variable m_filenameToImage&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::unordered_map</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::string, </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::shared_ptr</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> GUIImage </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> m_filenameToImage;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::unordered_map</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::string, </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::shared_ptr</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> GUIImage </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> m_filenameToImage;</span></span></code></pre></div><p>support structures, filename to <a href="/cpp-api-reference/classes/structGUIImage.html">GUIImage</a> (including GLuint id)</p><h3 id="variable-m-header" tabindex="-1">variable m_header <a class="header-anchor" href="#variable-m-header" aria-label="Permalink to &quot;variable m_header&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::shared_ptr</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> TutorialHeader </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> m_header;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::shared_ptr</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> TutorialHeader </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> m_header;</span></span></code></pre></div><h3 id="variable-m-steps" tabindex="-1">variable m_steps <a class="header-anchor" href="#variable-m-steps" aria-label="Permalink to &quot;variable m_steps&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::vector</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> TutorialStep </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> m_steps;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::vector</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> TutorialStep </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> m_steps;</span></span></code></pre></div><hr><p>Updated on 2025-01-07 at 09:46:55 +0000</p>`,21),o=[n];function p(r,c,i,d,u,h){return a(),t("div",null,o)}const m=s(l,[["render",p]]);export{E as __pageData,m as default};
