import{_ as s,o as a,c as n,Q as e}from"./chunks/framework.0f749972.js";const u=JSON.parse('{"title":"Command","description":"","frontmatter":{"title":"Command","summary":"Base class for all commands."},"headers":[],"relativePath":"cpp-api-reference/classes/classCommand.md","filePath":"cpp-api-reference/classes/classCommand.md"}'),o={name:"cpp-api-reference/classes/classCommand.md"},t=e(`<h1 id="command" tabindex="-1">Command <a class="header-anchor" href="#command" aria-label="Permalink to &quot;Command&quot;">​</a></h1><p>Base class for all commands. <a href="#detailed-description">More...</a></p><p><code>#include &quot;Commands/Command.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/classicommand.html">ICommand</a></p><h2 id="public-types" tabindex="-1">Public Types <a class="header-anchor" href="#public-types" aria-label="Permalink to &quot;Public Types&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>typedef std::function&lt; void(Args &amp;...)&gt;</td><td><strong><a href="/cpp-api-reference/classes/classcommand.html#typedef-callback">Callback</a></strong></td></tr></tbody></table><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classcommand.html#function-command">Command</a></strong>() =default</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classcommand.html#function-~command">~Command</a></strong>() =default</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classcommand.html#function-execute">execute</a></strong>() override&lt;br&gt;Call all callbacks.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classcommand.html#function-addlistener">addListener</a></strong>(<a href="/cpp-api-reference/classes/classcommand.html#typedef-callback">Callback</a> function)&lt;br&gt;Add a listener for a command.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classcommand.html#function-dispatch">dispatch</a></strong>(Args... args)&lt;br&gt;Tell application that command was issued.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classcommand.html#function-dispatchimmediate">dispatchImmediate</a></strong>(Args... args)</td></tr></tbody></table><h2 id="protected-functions" tabindex="-1">Protected Functions <a class="header-anchor" href="#protected-functions" aria-label="Permalink to &quot;Protected Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classcommand.html#function-command">Command</a></strong>(const std::tuple&lt; Args... &gt; &amp; args)&lt;br&gt;Copy a command and its arguments.</td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classicommand.html">ICommand</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classicommand.html#function-~icommand">~ICommand</a></strong>() =default</td></tr></tbody></table><h2 id="detailed-description" tabindex="-1">Detailed Description <a class="header-anchor" href="#detailed-description" aria-label="Permalink to &quot;Detailed Description&quot;">​</a></h2><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">template</span><span style="color:#E1E4E8;"> &lt;</span><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Type</span><span style="color:#E1E4E8;"> ,</span></span>
<span class="line"><span style="color:#F97583;">typename</span><span style="color:#E1E4E8;">... </span><span style="color:#B392F0;">Args</span><span style="color:#E1E4E8;">&gt;</span></span>
<span class="line"><span style="color:#F97583;">class</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Command</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">template</span><span style="color:#24292E;"> &lt;</span><span style="color:#D73A49;">typename</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Type</span><span style="color:#24292E;"> ,</span></span>
<span class="line"><span style="color:#D73A49;">typename</span><span style="color:#24292E;">... </span><span style="color:#6F42C1;">Args</span><span style="color:#24292E;">&gt;</span></span>
<span class="line"><span style="color:#D73A49;">class</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Command</span><span style="color:#24292E;">;</span></span></code></pre></div><p>Base class for all commands.</p><p><strong>Template Parameters</strong>:</p><ul><li><strong>Type</strong> command derived class (<a href="/cpp-api-reference/classes/classclosecommand.html">CloseCommand</a> for example)-</li><li><strong>Args</strong> command argument for callback call.</li></ul><p>Any derived command can be accessed by calling DerivedCommandName::addListener(fptr); For more information see <a href="https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern" target="_blank" rel="noreferrer">Curiously recurring template pattern (static m_listeners).</a></p><h2 id="public-types-documentation" tabindex="-1">Public Types Documentation <a class="header-anchor" href="#public-types-documentation" aria-label="Permalink to &quot;Public Types Documentation&quot;">​</a></h2><h3 id="typedef-callback" tabindex="-1">typedef Callback <a class="header-anchor" href="#typedef-callback" aria-label="Permalink to &quot;typedef Callback&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">typedef</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::function</span><span style="color:#F97583;">&lt;void</span><span style="color:#E1E4E8;">(Args</span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;">...)</span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Command</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">Type</span><span style="color:#E1E4E8;">, </span><span style="color:#B392F0;">Args</span><span style="color:#E1E4E8;"> &gt;::Callback;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">typedef</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::function</span><span style="color:#D73A49;">&lt;void</span><span style="color:#24292E;">(Args</span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;">...)</span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Command</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">Type</span><span style="color:#24292E;">, </span><span style="color:#6F42C1;">Args</span><span style="color:#24292E;"> &gt;::Callback;</span></span></code></pre></div><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-command" tabindex="-1">function Command <a class="header-anchor" href="#function-command" aria-label="Permalink to &quot;function Command&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Command</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">=default</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Command</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">=default</span></span></code></pre></div><h3 id="function-command-1" tabindex="-1">function ~Command <a class="header-anchor" href="#function-command-1" aria-label="Permalink to &quot;function ~Command&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">~</span><span style="color:#B392F0;">Command</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">=default</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">~</span><span style="color:#6F42C1;">Command</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">=default</span></span></code></pre></div><h3 id="function-execute" tabindex="-1">function execute <a class="header-anchor" href="#function-execute" aria-label="Permalink to &quot;function execute&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">execute</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">execute</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">override</span></span></code></pre></div><p>Call all callbacks.</p><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classicommand.html#function-execute">ICommand::execute</a></p><p>Expand m_params tuple as function arguments, <a href="https://stackoverflow.com/a/37100646" target="_blank" rel="noreferrer">link</a>.</p><h3 id="function-addlistener" tabindex="-1">function addListener <a class="header-anchor" href="#function-addlistener" aria-label="Permalink to &quot;function addListener&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">addListener</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Callback</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">function</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">addListener</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Callback</span><span style="color:#24292E;"> </span><span style="color:#E36209;">function</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Add a listener for a command.</p><p><strong>Parameters</strong>:</p><ul><li><strong>function</strong> callback to call when the command is issued. Use std::bind for member function of instantiated class or raw function pointer for static member function or plain function.</li></ul><p><strong>Todo</strong>: Check how std::bind handle <code>this</code> reference.</p><p><strong>NEVER</strong> use the function from constructor of statically instantiated object.</p><h3 id="function-dispatch" tabindex="-1">function dispatch <a class="header-anchor" href="#function-dispatch" aria-label="Permalink to &quot;function dispatch&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">dispatch</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Args</span><span style="color:#E1E4E8;">... </span><span style="color:#FFAB70;">args</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">dispatch</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Args</span><span style="color:#24292E;">... </span><span style="color:#E36209;">args</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Tell application that command was issued.</p><p>Pass a copy of command and its arguments to the <a href="/cpp-api-reference/classes/classapplication.html">Application</a>.</p><h3 id="function-dispatchimmediate" tabindex="-1">function dispatchImmediate <a class="header-anchor" href="#function-dispatchimmediate" aria-label="Permalink to &quot;function dispatchImmediate&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">dispatchImmediate</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Args</span><span style="color:#E1E4E8;">... </span><span style="color:#FFAB70;">args</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">dispatchImmediate</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Args</span><span style="color:#24292E;">... </span><span style="color:#E36209;">args</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h2 id="protected-functions-documentation" tabindex="-1">Protected Functions Documentation <a class="header-anchor" href="#protected-functions-documentation" aria-label="Permalink to &quot;Protected Functions Documentation&quot;">​</a></h2><h3 id="function-command-2" tabindex="-1">function Command <a class="header-anchor" href="#function-command-2" aria-label="Permalink to &quot;function Command&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">explicit</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Command</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::tuple</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> Args... </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> args</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">explicit</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Command</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::tuple</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> Args... </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> args</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Copy a command and its arguments.</p><p><strong>Parameters</strong>:</p><ul><li><strong>args</strong> command arguments.</li></ul><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>`,53),l=[t];function p(c,r,i,d,m,y){return a(),n("div",null,l)}const E=s(o,[["render",p]]);export{u as __pageData,E as default};
