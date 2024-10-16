import{_ as s,o as a,c as n,Q as e}from"./chunks/framework.0f749972.js";const h=JSON.parse('{"title":"ITutorialRenderer","description":"","frontmatter":{"title":"ITutorialRenderer"},"headers":[],"relativePath":"cpp-api-reference/classes/classITutorialRenderer.md","filePath":"cpp-api-reference/classes/classITutorialRenderer.md"}'),l={name:"cpp-api-reference/classes/classITutorialRenderer.md"},o=e(`<h1 id="itutorialrenderer" tabindex="-1">ITutorialRenderer <a class="header-anchor" href="#itutorialrenderer" aria-label="Permalink to &quot;ITutorialRenderer&quot;">​</a></h1><p><code>#include &quot;Tutorial/TutorialRenderer.h&quot;</code></p><p>Inherited by <a href="/cpp-api-reference/classes/classTutorialWindow.html">TutorialWindow</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classITutorialRenderer.html#function-~itutorialrenderer">~ITutorialRenderer</a></strong>() =default</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classITutorialRenderer.html#function-renderchoicetask">renderChoiceTask</a></strong>(<a href="/cpp-api-reference/classes/structChoiceTask.html">ChoiceTask</a> * choiceTask) =0</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classITutorialRenderer.html#function-renderexplanation">renderExplanation</a></strong>(<a href="/cpp-api-reference/classes/structExplanation.html">Explanation</a> * explanation) =0</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classITutorialRenderer.html#function-renderheadline">renderHeadline</a></strong>(<a href="/cpp-api-reference/classes/structHeadline.html">Headline</a> * headline) =0</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classITutorialRenderer.html#function-renderhint">renderHint</a></strong>(<a href="/cpp-api-reference/classes/structHint.html">Hint</a> * hint) =0</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classITutorialRenderer.html#function-renderinputtask">renderInputTask</a></strong>(<a href="/cpp-api-reference/classes/structInputTask.html">InputTask</a> * inputTask) =0</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classITutorialRenderer.html#function-rendermultichoicetask">renderMultiChoiceTask</a></strong>(<a href="/cpp-api-reference/classes/structMultiChoiceTask.html">MultiChoiceTask</a> * multiChoiceTask) =0</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classITutorialRenderer.html#function-rendertask">renderTask</a></strong>(<a href="/cpp-api-reference/classes/structTask.html">Task</a> * task) =0</td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-itutorialrenderer" tabindex="-1">function ~ITutorialRenderer <a class="header-anchor" href="#function-itutorialrenderer" aria-label="Permalink to &quot;function ~ITutorialRenderer&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">~</span><span style="color:#B392F0;">ITutorialRenderer</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">=default</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">~</span><span style="color:#6F42C1;">ITutorialRenderer</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">=default</span></span></code></pre></div><h3 id="function-renderchoicetask" tabindex="-1">function renderChoiceTask <a class="header-anchor" href="#function-renderchoicetask" aria-label="Permalink to &quot;function renderChoiceTask&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">renderChoiceTask</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">ChoiceTask</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">choiceTask</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">renderChoiceTask</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">ChoiceTask</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">choiceTask</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classTutorialWindow.html#function-renderchoicetask">TutorialWindow::renderChoiceTask</a></p><h3 id="function-renderexplanation" tabindex="-1">function renderExplanation <a class="header-anchor" href="#function-renderexplanation" aria-label="Permalink to &quot;function renderExplanation&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">renderExplanation</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Explanation</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">explanation</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">renderExplanation</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Explanation</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">explanation</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classTutorialWindow.html#function-renderexplanation">TutorialWindow::renderExplanation</a></p><h3 id="function-renderheadline" tabindex="-1">function renderHeadline <a class="header-anchor" href="#function-renderheadline" aria-label="Permalink to &quot;function renderHeadline&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">renderHeadline</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Headline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">headline</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">renderHeadline</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Headline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">headline</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classTutorialWindow.html#function-renderheadline">TutorialWindow::renderHeadline</a></p><h3 id="function-renderhint" tabindex="-1">function renderHint <a class="header-anchor" href="#function-renderhint" aria-label="Permalink to &quot;function renderHint&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">renderHint</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Hint</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">hint</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">renderHint</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Hint</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">hint</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classTutorialWindow.html#function-renderhint">TutorialWindow::renderHint</a></p><h3 id="function-renderinputtask" tabindex="-1">function renderInputTask <a class="header-anchor" href="#function-renderinputtask" aria-label="Permalink to &quot;function renderInputTask&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">renderInputTask</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">InputTask</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">inputTask</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">renderInputTask</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">InputTask</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">inputTask</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classTutorialWindow.html#function-renderinputtask">TutorialWindow::renderInputTask</a></p><h3 id="function-rendermultichoicetask" tabindex="-1">function renderMultiChoiceTask <a class="header-anchor" href="#function-rendermultichoicetask" aria-label="Permalink to &quot;function renderMultiChoiceTask&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">renderMultiChoiceTask</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">MultiChoiceTask</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">multiChoiceTask</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">renderMultiChoiceTask</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">MultiChoiceTask</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">multiChoiceTask</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classTutorialWindow.html#function-rendermultichoicetask">TutorialWindow::renderMultiChoiceTask</a></p><h3 id="function-rendertask" tabindex="-1">function renderTask <a class="header-anchor" href="#function-rendertask" aria-label="Permalink to &quot;function renderTask&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">renderTask</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Task</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">task</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">renderTask</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Task</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">task</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classTutorialWindow.html#function-rendertask">TutorialWindow::renderTask</a></p><hr><p>Updated on 2024-10-16 at 16:58:00 +0000</p>`,31),p=[o];function t(r,c,i,d,u,y){return a(),n("div",null,p)}const f=s(l,[["render",t]]);export{h as __pageData,f as default};