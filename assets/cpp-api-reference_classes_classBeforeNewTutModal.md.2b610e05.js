import{_ as t,o as e,c as a,Q as s}from"./chunks/framework.0f749972.js";const m=JSON.parse('{"title":"BeforeNewTutModal","description":"","frontmatter":{"title":"BeforeNewTutModal"},"headers":[],"relativePath":"cpp-api-reference/classes/classBeforeNewTutModal.md","filePath":"cpp-api-reference/classes/classBeforeNewTutModal.md"}'),n={name:"cpp-api-reference/classes/classBeforeNewTutModal.md"},r=s('<h1 id="beforenewtutmodal" tabindex="-1">BeforeNewTutModal <a class="header-anchor" href="#beforenewtutmodal" aria-label="Permalink to &quot;BeforeNewTutModal&quot;">​</a></h1><p><code>#include &quot;GUI/Elements/Modals/BeforeNewTutModal.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/classmodalwindow.html">ModalWindow</a>, <a href="/cpp-api-reference/classes/classiwindow.html">IWindow</a>, std::enable_shared_from_this&lt; IWindow &gt;</p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classbeforenewtutmodal.html#function-beforenewtutmodal">BeforeNewTutModal</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classbeforenewtutmodal.html#function-onimgui">onImGui</a></strong>() override</td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classmodalwindow.html">ModalWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classmodalwindow.html#function-modalwindow">ModalWindow</a></strong>(const std::string &amp; title)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classmodalwindow.html#function-close">close</a></strong>()&lt;br&gt;Hides this dialog and all its children.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classmodalwindow.html#function-open">open</a></strong>()</td></tr></tbody></table><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classiwindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-iwindow">IWindow</a></strong>(bool show =false)</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-~iwindow">~IWindow</a></strong>() =default</td></tr><tr><td>virtual const char *</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-getid">getID</a></strong>() const =0</td></tr><tr><td><a href="/cpp-api-reference/classes/classinputcontroller.html">InputController</a> &amp;</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-getinput">getInput</a></strong>()&lt;br&gt;Returns window input controller.</td></tr><tr><td><a href="/cpp-api-reference/classes/classinputcontroller.html">InputController</a> *</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-getinputptr">getInputPtr</a></strong>()</td></tr><tr><td>const char *</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-getname">getName</a></strong>() const</td></tr><tr><td>bool *</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-getshowptr">getShowPtr</a></strong>()</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-hide">hide</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-isvisible">isVisible</a></strong>() const</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-render">render</a></strong>() =0</td></tr><tr><td>const std::string &amp;</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-setname">setName</a></strong>(const char * name)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-show">show</a></strong>()</td></tr></tbody></table><p><strong>Protected Functions inherited from <a href="/cpp-api-reference/classes/classiwindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#function-updatewindowinfo">updateWindowInfo</a></strong>()&lt;br&gt;Collect information about the current window.</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classiwindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>glm::vec2</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#variable-m-windowmax">m_windowMax</a></strong> &lt;br&gt;Bottom right corner of the window.</td></tr><tr><td>glm::vec2</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#variable-m-windowmin">m_windowMin</a></strong> &lt;br&gt;Top left corner of the window, same as m_windowPos (separate variable for clarity)</td></tr><tr><td>glm::vec2</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#variable-m-windowpos">m_windowPos</a></strong> &lt;br&gt;Top-left corner of the window in screen coordinates.</td></tr><tr><td>glm::ivec2</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#variable-m-windowsize">m_windowSize</a></strong> &lt;br&gt;<a href="/cpp-api-reference/classes/classwindow.html">Window</a> width and height dimensions.</td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classiwindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#variable-imguiname">imGuiName</a></strong></td></tr><tr><td><a href="/cpp-api-reference/classes/classinputcontroller.html">InputController</a></td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#variable-input">Input</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#variable-m-show">m_show</a></strong></td></tr><tr><td><a href="/cpp-api-reference/classes/classwindowmanager.html">WindowManager</a> *</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#variable-m-windowmanager">m_windowManager</a></strong> &lt;br&gt;Weak reference to a <a href="/cpp-api-reference/classes/classwindowmanager.html">WindowManager</a> set when this window is added to it.</td></tr></tbody></table><p><strong>Friends inherited from <a href="/cpp-api-reference/classes/classiwindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>class</td><td><strong><a href="/cpp-api-reference/classes/classiwindow.html#friend-windowmanager">WindowManager</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-beforenewtutmodal" tabindex="-1">function BeforeNewTutModal <a class="header-anchor" href="#function-beforenewtutmodal" aria-label="Permalink to &quot;function BeforeNewTutModal&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">BeforeNewTutModal</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">BeforeNewTutModal</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-onimgui" tabindex="-1">function onImGui <a class="header-anchor" href="#function-onimgui" aria-label="Permalink to &quot;function onImGui&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">onImGui</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">onImGui</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">override</span></span></code></pre></div><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classmodalwindow.html#function-onimgui">ModalWindow::onImGui</a></p><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>',26),o=[r];function i(l,d,c,p,h,f){return e(),a("div",null,o)}const u=t(n,[["render",i]]);export{m as __pageData,u as default};
