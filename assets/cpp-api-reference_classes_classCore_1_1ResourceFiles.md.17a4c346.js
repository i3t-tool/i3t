import{_ as s,o as a,c as e,Q as l}from"./chunks/framework.0f749972.js";const E=JSON.parse('{"title":"Core::ResourceFiles","description":"","frontmatter":{"title":"Core::ResourceFiles","summary":"A collection of file paths in the filesystem that are together required to load a single resource like a model."},"headers":[],"relativePath":"cpp-api-reference/classes/classCore_1_1ResourceFiles.md","filePath":"cpp-api-reference/classes/classCore_1_1ResourceFiles.md"}'),o={name:"cpp-api-reference/classes/classCore_1_1ResourceFiles.md"},n=l(`<h1 id="core-resourcefiles" tabindex="-1">Core::ResourceFiles <a class="header-anchor" href="#core-resourcefiles" aria-label="Permalink to &quot;Core::ResourceFiles&quot;">​</a></h1><p>A collection of file paths in the filesystem that are together required to load a single resource like a model. <a href="#detailed-description">More...</a></p><p><code>#include &quot;Core/Resources/ResourceFiles.h&quot;</code></p><p>Inherited by <a href="/cpp-api-reference/classes/classCore_1_1ModelResourceFiles.html">Core::ModelResourceFiles</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-resourcefiles">ResourceFiles</a></strong>(fs::path path, std::string name)</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-~resourcefiles">~ResourceFiles</a></strong>() =default</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-addfile">addFile</a></strong>(fs::path filePath)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-changelocation">changeLocation</a></strong>(fs::path newDir, bool deleteOriginals)&lt;br&gt;Change the location of the managed files.</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-deletefiles">deleteFiles</a></strong>()</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-fetchfiles">fetchFiles</a></strong>() =0&lt;br&gt;Derive and fetch all required files from the primary file path.</td></tr><tr><td>virtual bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-fetchfiles">fetchFiles</a></strong>(void * data) =0&lt;br&gt;Derive and fetch all required files from the primary file path and supplied arbitrary data.</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-operator!=">operator!=</a></strong>(const <a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html">ResourceFiles</a> &amp; rhs) const</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-operator&lt;">operator&lt;</a></strong>(const <a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html">ResourceFiles</a> &amp; rhs) const</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#function-operator==">operator==</a></strong>(const <a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html">ResourceFiles</a> &amp; rhs) const</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>std::set&lt; fs::path &gt;</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#variable-m-files">m_files</a></strong></td></tr><tr><td>std::string</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#variable-m-name">m_name</a></strong> &lt;br&gt;Arbitrary name, can be used as a name for the root subfolder.</td></tr><tr><td>fs::path</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#variable-m-path">m_path</a></strong> &lt;br&gt;Path to the &quot;primary&quot; file, from which other paths are derived.</td></tr><tr><td>fs::path</td><td><strong><a href="/cpp-api-reference/classes/classCore_1_1ResourceFiles.html#variable-m-rootdir">m_rootDir</a></strong> &lt;br&gt;Root directory of the resource. Derived from m_path.</td></tr></tbody></table><h2 id="detailed-description" tabindex="-1">Detailed Description <a class="header-anchor" href="#detailed-description" aria-label="Permalink to &quot;Detailed Description&quot;">​</a></h2><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">class</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Core</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">ResourceFiles</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">class</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Core</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">ResourceFiles</span><span style="color:#24292E;">;</span></span></code></pre></div><p>A collection of file paths in the filesystem that are together required to load a single resource like a model.</p><p>An instance of this class helps keep track of actual files that a resource was loaded from and allows them to be deleted when not used anymore.</p><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-resourcefiles" tabindex="-1">function ResourceFiles <a class="header-anchor" href="#function-resourcefiles" aria-label="Permalink to &quot;function ResourceFiles&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">ResourceFiles</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path path,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::string name</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">ResourceFiles</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path path,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::string name</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-resourcefiles-1" tabindex="-1">function ~ResourceFiles <a class="header-anchor" href="#function-resourcefiles-1" aria-label="Permalink to &quot;function ~ResourceFiles&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">~</span><span style="color:#B392F0;">ResourceFiles</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">=default</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">~</span><span style="color:#6F42C1;">ResourceFiles</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">=default</span></span></code></pre></div><h3 id="function-addfile" tabindex="-1">function addFile <a class="header-anchor" href="#function-addfile" aria-label="Permalink to &quot;function addFile&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">addFile</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">path</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">filePath</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">addFile</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">path</span><span style="color:#24292E;"> </span><span style="color:#E36209;">filePath</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-changelocation" tabindex="-1">function changeLocation <a class="header-anchor" href="#function-changelocation" aria-label="Permalink to &quot;function changeLocation&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">changeLocation</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">path</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">newDir</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">deleteOriginals</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">changeLocation</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">path</span><span style="color:#24292E;"> </span><span style="color:#E36209;">newDir</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#E36209;">deleteOriginals</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Change the location of the managed files.</p><p><strong>Parameters</strong>:</p><ul><li><strong>newDir</strong> New file structure root.</li><li><strong>deleteOriginals</strong> Delete the original files.</li></ul><p><strong>Return</strong>: True on success, false otherwise</p><h3 id="function-deletefiles" tabindex="-1">function deleteFiles <a class="header-anchor" href="#function-deletefiles" aria-label="Permalink to &quot;function deleteFiles&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">deleteFiles</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">deleteFiles</span><span style="color:#24292E;">()</span></span></code></pre></div><h3 id="function-fetchfiles" tabindex="-1">function fetchFiles <a class="header-anchor" href="#function-fetchfiles" aria-label="Permalink to &quot;function fetchFiles&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fetchFiles</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fetchFiles</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span></code></pre></div><p>Derive and fetch all required files from the primary file path.</p><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classCore_1_1ModelResourceFiles.html#function-fetchfiles">Core::ModelResourceFiles::fetchFiles</a></p><h3 id="function-fetchfiles-1" tabindex="-1">function fetchFiles <a class="header-anchor" href="#function-fetchfiles-1" aria-label="Permalink to &quot;function fetchFiles&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fetchFiles</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">*</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">data</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">=</span><span style="color:#79B8FF;">0</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fetchFiles</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">*</span><span style="color:#24292E;"> </span><span style="color:#E36209;">data</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">=</span><span style="color:#005CC5;">0</span></span></code></pre></div><p>Derive and fetch all required files from the primary file path and supplied arbitrary data.</p><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classCore_1_1ModelResourceFiles.html#function-fetchfiles">Core::ModelResourceFiles::fetchFiles</a></p><h3 id="function-operator" tabindex="-1">function operator!= <a class="header-anchor" href="#function-operator" aria-label="Permalink to &quot;function operator!=&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">operator</span><span style="color:#B392F0;">!=</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ResourceFiles</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">rhs</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">operator</span><span style="color:#6F42C1;">!=</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ResourceFiles</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">rhs</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">const</span></span></code></pre></div><h3 id="function-operator-1" tabindex="-1">function operator&lt; <a class="header-anchor" href="#function-operator-1" aria-label="Permalink to &quot;function operator&amp;lt;&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">operator</span><span style="color:#B392F0;">&lt;</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ResourceFiles</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">rhs</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">operator</span><span style="color:#6F42C1;">&lt;</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ResourceFiles</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">rhs</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">const</span></span></code></pre></div><h3 id="function-operator-2" tabindex="-1">function operator== <a class="header-anchor" href="#function-operator-2" aria-label="Permalink to &quot;function operator==&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">inline</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">operator</span><span style="color:#B392F0;">==</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ResourceFiles</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">rhs</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">inline</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">operator</span><span style="color:#6F42C1;">==</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ResourceFiles</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">rhs</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">const</span></span></code></pre></div><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-files" tabindex="-1">variable m_files <a class="header-anchor" href="#variable-m-files" aria-label="Permalink to &quot;variable m_files&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::set</span><span style="color:#F97583;">&lt;</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path </span><span style="color:#F97583;">&gt;</span><span style="color:#E1E4E8;"> m_files;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::set</span><span style="color:#D73A49;">&lt;</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path </span><span style="color:#D73A49;">&gt;</span><span style="color:#24292E;"> m_files;</span></span></code></pre></div><h3 id="variable-m-name" tabindex="-1">variable m_name <a class="header-anchor" href="#variable-m-name" aria-label="Permalink to &quot;variable m_name&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::string m_name;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::string m_name;</span></span></code></pre></div><p>Arbitrary name, can be used as a name for the root subfolder.</p><h3 id="variable-m-path" tabindex="-1">variable m_path <a class="header-anchor" href="#variable-m-path" aria-label="Permalink to &quot;variable m_path&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path m_path;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path m_path;</span></span></code></pre></div><p>Path to the &quot;primary&quot; file, from which other paths are derived.</p><h3 id="variable-m-rootdir" tabindex="-1">variable m_rootDir <a class="header-anchor" href="#variable-m-rootdir" aria-label="Permalink to &quot;variable m_rootDir&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">fs</span><span style="color:#E1E4E8;">::path m_rootDir;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">fs</span><span style="color:#24292E;">::path m_rootDir;</span></span></code></pre></div><p>Root directory of the resource. Derived from m_path.</p><hr><p>Updated on 2024-10-16 at 16:44:51 +0000</p>`,55),p=[n];function t(r,c,i,d,h,y){return a(),e("div",null,p)}const f=s(o,[["render",t]]);export{E as __pageData,f as default};