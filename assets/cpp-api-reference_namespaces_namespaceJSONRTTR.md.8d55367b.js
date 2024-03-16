import{_ as s,o as a,c as n,Q as p}from"./chunks/framework.0f749972.js";const v=JSON.parse('{"title":"JSONRTTR","description":"","frontmatter":{"title":"JSONRTTR"},"headers":[],"relativePath":"cpp-api-reference/namespaces/namespaceJSONRTTR.md","filePath":"cpp-api-reference/namespaces/namespaceJSONRTTR.md"}'),o={name:"cpp-api-reference/namespaces/namespaceJSONRTTR.md"},l=p(`<h1 id="jsonrttr" tabindex="-1">JSONRTTR <a class="header-anchor" href="#jsonrttr" aria-label="Permalink to &quot;JSONRTTR&quot;">​</a></h1><h2 id="functions" tabindex="-1">Functions <a class="header-anchor" href="#functions" aria-label="Permalink to &quot;Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>variant</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-extract-basic-types">extract_basic_types</a></strong>(const <a href="/cpp-api-reference/files/workspaceelements_8h.html#enumvalue-value">Value</a> &amp; json_value)</td></tr><tr><td>variant</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-extract-value">extract_value</a></strong>(Value::ConstMemberIterator &amp; itr, const type &amp; t)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-fromjson-recursively">fromjson_recursively</a></strong>(instance obj, const <a href="/cpp-api-reference/files/workspaceelements_8h.html#enumvalue-value">Value</a> &amp; json_object)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-to-json-recursively">to_json_recursively</a></strong>(const instance &amp; obj, PrettyWriter&lt; StringBuffer &gt; &amp; writer)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-write-array">write_array</a></strong>(const variant_sequential_view &amp; view, PrettyWriter&lt; StringBuffer &gt; &amp; writer)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-write-array-recursively">write_array_recursively</a></strong>(variant_sequential_view &amp; view, const <a href="/cpp-api-reference/files/workspaceelements_8h.html#enumvalue-value">Value</a> &amp; json_array_value)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-write-associative-container">write_associative_container</a></strong>(const variant_associative_view &amp; view, PrettyWriter&lt; StringBuffer &gt; &amp; writer)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-write-associative-view-recursively">write_associative_view_recursively</a></strong>(variant_associative_view &amp; view, const <a href="/cpp-api-reference/files/workspaceelements_8h.html#enumvalue-value">Value</a> &amp; json_array_value)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-write-atomic-types-to-json">write_atomic_types_to_json</a></strong>(const type &amp; t, const variant &amp; var, PrettyWriter&lt; StringBuffer &gt; &amp; writer)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/namespaces/namespacejsonrttr.html#function-write-variant">write_variant</a></strong>(const variant &amp; var, PrettyWriter&lt; StringBuffer &gt; &amp; writer)</td></tr></tbody></table><h2 id="functions-documentation" tabindex="-1">Functions Documentation <a class="header-anchor" href="#functions-documentation" aria-label="Permalink to &quot;Functions Documentation&quot;">​</a></h2><h3 id="function-extract-basic-types" tabindex="-1">function extract_basic_types <a class="header-anchor" href="#function-extract-basic-types" aria-label="Permalink to &quot;function extract_basic_types&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">variant</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">extract_basic_types</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Value</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">json_value</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">variant</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">extract_basic_types</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Value</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">json_value</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-extract-value" tabindex="-1">function extract_value <a class="header-anchor" href="#function-extract-value" aria-label="Permalink to &quot;function extract_value&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">variant</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">extract_value</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Value</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">ConstMemberIterator</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">itr</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">type</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">t</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">variant</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">extract_value</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Value</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">ConstMemberIterator</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">itr</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">type</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">t</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-fromjson-recursively" tabindex="-1">function fromjson_recursively <a class="header-anchor" href="#function-fromjson-recursively" aria-label="Permalink to &quot;function fromjson_recursively&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">fromjson_recursively</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">instance</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">obj</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Value</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">json_object</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">fromjson_recursively</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">instance</span><span style="color:#24292E;"> </span><span style="color:#E36209;">obj</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Value</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">json_object</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-to-json-recursively" tabindex="-1">function to_json_recursively <a class="header-anchor" href="#function-to-json-recursively" aria-label="Permalink to &quot;function to_json_recursively&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">to_json_recursively</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">instance</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">obj</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">PrettyWriter</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">StringBuffer</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">writer</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">to_json_recursively</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">instance</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">obj</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">PrettyWriter</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">StringBuffer</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">writer</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-write-array" tabindex="-1">function write_array <a class="header-anchor" href="#function-write-array" aria-label="Permalink to &quot;function write_array&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">write_array</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">variant_sequential_view</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">view</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">PrettyWriter</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">StringBuffer</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">writer</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">write_array</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">variant_sequential_view</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">view</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">PrettyWriter</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">StringBuffer</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">writer</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-write-array-recursively" tabindex="-1">function write_array_recursively <a class="header-anchor" href="#function-write-array-recursively" aria-label="Permalink to &quot;function write_array_recursively&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">write_array_recursively</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">variant_sequential_view</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">view</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Value</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">json_array_value</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">write_array_recursively</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">variant_sequential_view</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">view</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Value</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">json_array_value</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-write-associative-container" tabindex="-1">function write_associative_container <a class="header-anchor" href="#function-write-associative-container" aria-label="Permalink to &quot;function write_associative_container&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">write_associative_container</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">variant_associative_view</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">view</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">PrettyWriter</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">StringBuffer</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">writer</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">write_associative_container</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">variant_associative_view</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">view</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">PrettyWriter</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">StringBuffer</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">writer</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-write-associative-view-recursively" tabindex="-1">function write_associative_view_recursively <a class="header-anchor" href="#function-write-associative-view-recursively" aria-label="Permalink to &quot;function write_associative_view_recursively&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">static</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">write_associative_view_recursively</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">variant_associative_view</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">view</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Value</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">json_array_value</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">static</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">write_associative_view_recursively</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">variant_associative_view</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">view</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Value</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">json_array_value</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-write-atomic-types-to-json" tabindex="-1">function write_atomic_types_to_json <a class="header-anchor" href="#function-write-atomic-types-to-json" aria-label="Permalink to &quot;function write_atomic_types_to_json&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">write_atomic_types_to_json</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">type</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">t</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">variant</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">var</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">PrettyWriter</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">StringBuffer</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">writer</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">write_atomic_types_to_json</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">type</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">t</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">variant</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">var</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">PrettyWriter</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">StringBuffer</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">writer</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-write-variant" tabindex="-1">function write_variant <a class="header-anchor" href="#function-write-variant" aria-label="Permalink to &quot;function write_variant&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">write_variant</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">variant</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">var</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">PrettyWriter</span><span style="color:#E1E4E8;">&lt; </span><span style="color:#B392F0;">StringBuffer</span><span style="color:#E1E4E8;"> &gt; </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">writer</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">write_variant</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">variant</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">var</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">PrettyWriter</span><span style="color:#24292E;">&lt; </span><span style="color:#6F42C1;">StringBuffer</span><span style="color:#24292E;"> &gt; </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">writer</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>`,26),e=[l];function t(r,c,i,y,E,u){return a(),n("div",null,e)}const h=s(o,[["render",t]]);export{v as __pageData,h as default};
