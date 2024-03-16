import{_ as s,o as e,c as t,Q as a}from"./chunks/framework.0f749972.js";const y=JSON.parse('{"title":"ChoiceTask","description":"","frontmatter":{"title":"ChoiceTask"},"headers":[],"relativePath":"cpp-api-reference/classes/structChoiceTask.md","filePath":"cpp-api-reference/classes/structChoiceTask.md"}'),n={name:"cpp-api-reference/classes/structChoiceTask.md"},o=a(`<h1 id="choicetask" tabindex="-1">ChoiceTask <a class="header-anchor" href="#choicetask" aria-label="Permalink to &quot;ChoiceTask&quot;">​</a></h1><p><code>#include &quot;Tutorial/Tutorial.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/structtutorialelement.html">TutorialElement</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/structchoicetask.html#function-choicetask">ChoiceTask</a></strong>(std::string question, std::vector&lt; std::string &gt; choices, int correctChoice)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/structchoicetask.html#function-acceptrenderer">acceptRenderer</a></strong>(<a href="/cpp-api-reference/classes/classitutorialrenderer.html">ITutorialRenderer</a> * tutorialRenderer) override</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>std::vector&lt; std::string &gt;</td><td><strong><a href="/cpp-api-reference/classes/structchoicetask.html#variable-m-choices">m_choices</a></strong></td></tr><tr><td>int</td><td><strong><a href="/cpp-api-reference/classes/structchoicetask.html#variable-m-correctchoice">m_correctChoice</a></strong></td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/structtutorialelement.html">TutorialElement</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/structtutorialelement.html#function-tutorialelement">TutorialElement</a></strong>()</td></tr><tr><td></td><td><strong><a href="/cpp-api-reference/classes/structtutorialelement.html#function-tutorialelement">TutorialElement</a></strong>(std::string content)</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/structtutorialelement.html#function-~tutorialelement">~TutorialElement</a></strong>() =default</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/structtutorialelement.html">TutorialElement</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/structtutorialelement.html#variable-m-content">m_content</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-choicetask" tabindex="-1">function ChoiceTask <a class="header-anchor" href="#function-choicetask" aria-label="Permalink to &quot;function ChoiceTask&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ChoiceTask</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::string question,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::vector</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::string </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> choices,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">int</span><span style="color:#E1E4E8;"> correctChoice</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ChoiceTask</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::string question,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::vector</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::string </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> choices,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">int</span><span style="color:#24292E;"> correctChoice</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-acceptrenderer" tabindex="-1">function acceptRenderer <a class="header-anchor" href="#function-acceptrenderer" aria-label="Permalink to &quot;function acceptRenderer&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">acceptRenderer</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">ITutorialRenderer</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">tutorialRenderer</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">acceptRenderer</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">ITutorialRenderer</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">tutorialRenderer</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">override</span></span></code></pre></div><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/structtutorialelement.html#function-acceptrenderer">TutorialElement::acceptRenderer</a></p><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-choices" tabindex="-1">variable m_choices <a class="header-anchor" href="#variable-m-choices" aria-label="Permalink to &quot;variable m_choices&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::vector</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::string </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> m_choices;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::vector</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::string </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> m_choices;</span></span></code></pre></div><h3 id="variable-m-correctchoice" tabindex="-1">variable m_correctChoice <a class="header-anchor" href="#variable-m-correctchoice" aria-label="Permalink to &quot;variable m_correctChoice&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">int</span><span style="color:#E1E4E8;"> m_correctChoice;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">int</span><span style="color:#24292E;"> m_correctChoice;</span></span></code></pre></div><hr><p>Updated on 2024-03-16 at 18:59:44 +0000</p>`,25),l=[o];function c(r,p,i,d,h,u){return e(),t("div",null,l)}const m=s(n,[["render",c]]);export{y as __pageData,m as default};
