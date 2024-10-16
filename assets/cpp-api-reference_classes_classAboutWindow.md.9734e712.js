import{_ as t,o as e,c as a,Q as s}from"./chunks/framework.0f749972.js";const f=JSON.parse('{"title":"AboutWindow","description":"","frontmatter":{"title":"AboutWindow"},"headers":[],"relativePath":"cpp-api-reference/classes/classAboutWindow.md","filePath":"cpp-api-reference/classes/classAboutWindow.md"}'),r={name:"cpp-api-reference/classes/classAboutWindow.md"},n=s('<h1 id="aboutwindow" tabindex="-1">AboutWindow <a class="header-anchor" href="#aboutwindow" aria-label="Permalink to &quot;AboutWindow&quot;">​</a></h1><p><code>#include &quot;GUI/Elements/Windows/AboutWindow.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/classIWindow.html">IWindow</a>, std::enable_shared_from_this&lt; IWindow &gt;</p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classAboutWindow.html#function-aboutwindow">AboutWindow</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classAboutWindow.html#function-render">render</a></strong>() override</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>std::shared_ptr&lt; TextureRegion &gt;</td><td><strong><a href="/cpp-api-reference/classes/classAboutWindow.html#variable-dcgireg">dcgiReg</a></strong></td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classIWindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-iwindow">IWindow</a></strong>(std::string title, bool show =false)</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-~iwindow">~IWindow</a></strong>() =default</td></tr><tr><td>virtual const char *</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-getid">getID</a></strong>() const =0</td></tr><tr><td><a href="/cpp-api-reference/files/Types_8h.html#using-wptr">WPtr</a>&lt; <a href="/cpp-api-reference/classes/classInputController.html">InputController</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-getinput">getInput</a></strong>()&lt;br&gt;Returns window input controller.</td></tr><tr><td>const char *</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-getname">getName</a></strong>()</td></tr><tr><td>bool *</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-getshowptr">getShowPtr</a></strong>()</td></tr><tr><td>const std::string &amp;</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-gettitle">getTitle</a></strong>() const</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-hide">hide</a></strong>()</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-isvisible">isVisible</a></strong>() const</td></tr><tr><td>const void</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-settitle">setTitle</a></strong>(std::string title)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-show">show</a></strong>()</td></tr></tbody></table><p><strong>Protected Functions inherited from <a href="/cpp-api-reference/classes/classIWindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#function-updatewindowinfo">updateWindowInfo</a></strong>()&lt;br&gt;Collect information about the current window.</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classIWindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-autofocus">m_autoFocus</a></strong> &lt;br&gt;When true the window will get focus immediately upon hovering over it.</td></tr><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-title">m_title</a></strong> &lt;br&gt;Visible window title.</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-windowinfoupdated">m_windowInfoUpdated</a></strong> &lt;br&gt;A flag indicating the <a href="./.html">updateWindowInfo()</a> method was called.</td></tr><tr><td><a href="/cpp-api-reference/classes/classWindowManager.html">WindowManager</a> *</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-windowmanager">m_windowManager</a></strong> &lt;br&gt;Weak reference to a <a href="/cpp-api-reference/classes/classWindowManager.html">WindowManager</a> set when this window is added to it.</td></tr><tr><td>glm::vec2</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-windowmax">m_windowMax</a></strong> &lt;br&gt;Bottom right corner of the window.</td></tr><tr><td>glm::vec2</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-windowmin">m_windowMin</a></strong> &lt;br&gt;Top left corner of the window, same as m_windowPos (separate variable for clarity)</td></tr><tr><td>glm::vec2</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-windowpos">m_windowPos</a></strong> &lt;br&gt;Top-left corner of the window in screen coordinates.</td></tr><tr><td>glm::ivec2</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-windowsize">m_windowSize</a></strong> &lt;br&gt;<a href="/cpp-api-reference/classes/classWindow.html">Window</a> width and height dimensions.</td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classIWindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td><a href="/cpp-api-reference/files/Types_8h.html#using-ptr">Ptr</a>&lt; <a href="/cpp-api-reference/classes/classInputController.html">InputController</a> &gt;</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-input">m_input</a></strong></td></tr><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-name">m_name</a></strong> &lt;br&gt;Full ImGui window identifier.</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-nameneedsupdate">m_nameNeedsUpdate</a></strong> &lt;br&gt;Workaround to avoid setting the name in the constructor (cannot call getID there)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#variable-m-show">m_show</a></strong></td></tr></tbody></table><p><strong>Friends inherited from <a href="/cpp-api-reference/classes/classIWindow.html">IWindow</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>class</td><td><strong><a href="/cpp-api-reference/classes/classIWindow.html#friend-windowmanager">WindowManager</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-aboutwindow" tabindex="-1">function AboutWindow <a class="header-anchor" href="#function-aboutwindow" aria-label="Permalink to &quot;function AboutWindow&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">AboutWindow</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">AboutWindow</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-render" tabindex="-1">function render <a class="header-anchor" href="#function-render" aria-label="Permalink to &quot;function render&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">render</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">render</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">override</span></span></code></pre></div><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classIWindow.html#function-render">IWindow::render</a></p><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-dcgireg" tabindex="-1">variable dcgiReg <a class="header-anchor" href="#variable-dcgireg" aria-label="Permalink to &quot;variable dcgiReg&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::shared_ptr</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> TextureRegion </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> dcgiReg;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::shared_ptr</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> TextureRegion </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> dcgiReg;</span></span></code></pre></div><hr><p>Updated on 2024-10-16 at 16:58:00 +0000</p>',29),o=[n];function i(d,l,c,p,h,u){return e(),a("div",null,o)}const b=t(r,[["render",i]]);export{f as __pageData,b as default};