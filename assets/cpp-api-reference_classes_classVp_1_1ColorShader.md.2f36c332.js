import{_ as e,o as t,c as a,Q as s}from"./chunks/framework.0f749972.js";const m=JSON.parse('{"title":"Vp::ColorShader","description":"","frontmatter":{"title":"Vp::ColorShader","summary":"Basic shader to display unlit colored primitives."},"headers":[],"relativePath":"cpp-api-reference/classes/classVp_1_1ColorShader.md","filePath":"cpp-api-reference/classes/classVp_1_1ColorShader.md"}'),r={name:"cpp-api-reference/classes/classVp_1_1ColorShader.md"},o=s(`<h1 id="vp-colorshader" tabindex="-1">Vp::ColorShader <a class="header-anchor" href="#vp-colorshader" aria-label="Permalink to &quot;Vp::ColorShader&quot;">​</a></h1><p>Basic shader to display unlit colored primitives.</p><p><code>#include &quot;Viewport/shader/ColorShader.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html">Vp::ObjectShader</a>, <a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html">Vp::WorldShader</a>, <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></p><p>Inherited by <a href="/cpp-api-reference/classes/classVp_1_1FrustumShader.html">Vp::FrustumShader</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ColorShader.html#function-colorshader">ColorShader</a></strong>(GLuint id)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ColorShader.html#function-init">init</a></strong>(bool initSuperclass) override</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ColorShader.html#function-setuniforms">setUniforms</a></strong>() override&lt;br&gt;Set uniforms before rendering.</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>glm::vec3</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ColorShader.html#variable-m-singlecolor">m_singleColor</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ColorShader.html#variable-m-usesinglecolor">m_useSingleColor</a></strong></td></tr></tbody></table><h2 id="protected-attributes" tabindex="-1">Protected Attributes <a class="header-anchor" href="#protected-attributes" aria-label="Permalink to &quot;Protected Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ColorShader.html#variable-singlecolorid">singleColorId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ColorShader.html#variable-usesinglecolorid">useSingleColorId</a></strong></td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html">Vp::ObjectShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html#function-objectshader">ObjectShader</a></strong>(GLuint id)</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html">Vp::ObjectShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>float</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html#variable-m-opacity">m_opacity</a></strong></td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html">Vp::ObjectShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html#variable-opacityid">opacityId</a></strong></td></tr></tbody></table><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html">Vp::WorldShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#function-worldshader">WorldShader</a></strong>(GLuint id)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#function-setworldtransform">setWorldTransform</a></strong>(const glm::mat4 &amp; model, const glm::mat4 &amp; view, const glm::mat4 &amp; projection)&lt;br&gt;Updates MVP matrices.</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html">Vp::WorldShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>glm::mat4</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-model">m_model</a></strong></td></tr><tr><td>glm::mat4</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-projection">m_projection</a></strong></td></tr><tr><td>glm::mat4</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-view">m_view</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-wboitfarid">m_wboitFarId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-wboitnearid">m_wboitNearId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-modelmatrixid">modelMatrixId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-normalmatrixid">normalMatrixId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-projectionmatrixid">projectionMatrixId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-pvmmatrixid">pvmMatrixId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-viewmatrixid">viewMatrixId</a></strong></td></tr></tbody></table><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-shader">Shader</a></strong>(GLuint id)</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-~shader">~Shader</a></strong>() =default</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-bindtexture2d">bindTexture2D</a></strong>(GLuint textureUnit, GLuint textureID, GLint samplerLocation)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-bindtexture2dms">bindTexture2DMS</a></strong>(GLuint textureUnit, GLuint textureID, GLint samplerLocation)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-hasuniform">hasUniform</a></strong>(GLint location)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-setuniformspermeshpart">setUniformsPerMeshPart</a></strong>(<a href="/cpp-api-reference/classes/structCore_1_1Mesh_1_1MeshPart.html">Core::Mesh::MeshPart</a> &amp; meshPart)&lt;br&gt;Set uniforms before rendering individual mesh parts.</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-supportswboit">supportsWboit</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-use">use</a></strong>() const</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLuint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-id">m_id</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboit">m_wboit</a></strong> &lt;br&gt;Whether to use weighted blended transparency for output if supported.</td></tr><tr><td>int</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboitfunc">m_wboitFunc</a></strong></td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboitflagid">m_wboitFlagId</a></strong> &lt;br&gt;Uniform id of the wboit enable/disable flag.</td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboitfuncid">m_wboitFuncId</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-colorshader" tabindex="-1">function ColorShader <a class="header-anchor" href="#function-colorshader" aria-label="Permalink to &quot;function ColorShader&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">explicit</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">ColorShader</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    GLuint id</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">explicit</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">ColorShader</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    GLuint id</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-init" tabindex="-1">function init <a class="header-anchor" href="#function-init" aria-label="Permalink to &quot;function init&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">init</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">initSuperclass</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">init</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#E36209;">initSuperclass</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">override</span></span></code></pre></div><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-init">Vp::Shader::init</a></p><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classVp_1_1FrustumShader.html#function-init">Vp::FrustumShader::init</a></p><h3 id="function-setuniforms" tabindex="-1">function setUniforms <a class="header-anchor" href="#function-setuniforms" aria-label="Permalink to &quot;function setUniforms&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setUniforms</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setUniforms</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">override</span></span></code></pre></div><p>Set uniforms before rendering.</p><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-setuniforms">Vp::Shader::setUniforms</a></p><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classVp_1_1FrustumShader.html#function-setuniforms">Vp::FrustumShader::setUniforms</a></p><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-singlecolor" tabindex="-1">variable m_singleColor <a class="header-anchor" href="#variable-m-singlecolor" aria-label="Permalink to &quot;variable m_singleColor&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::vec3 m_singleColor {</span><span style="color:#79B8FF;">1.0</span><span style="color:#F97583;">f</span><span style="color:#E1E4E8;">, </span><span style="color:#79B8FF;">0.0</span><span style="color:#F97583;">f</span><span style="color:#E1E4E8;">, </span><span style="color:#79B8FF;">1.0</span><span style="color:#F97583;">f</span><span style="color:#E1E4E8;">};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::vec3 m_singleColor {</span><span style="color:#005CC5;">1.0</span><span style="color:#D73A49;">f</span><span style="color:#24292E;">, </span><span style="color:#005CC5;">0.0</span><span style="color:#D73A49;">f</span><span style="color:#24292E;">, </span><span style="color:#005CC5;">1.0</span><span style="color:#D73A49;">f</span><span style="color:#24292E;">};</span></span></code></pre></div><h3 id="variable-m-usesinglecolor" tabindex="-1">variable m_useSingleColor <a class="header-anchor" href="#variable-m-usesinglecolor" aria-label="Permalink to &quot;variable m_useSingleColor&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> m_useSingleColor {</span><span style="color:#79B8FF;">false</span><span style="color:#E1E4E8;">};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> m_useSingleColor {</span><span style="color:#005CC5;">false</span><span style="color:#24292E;">};</span></span></code></pre></div><h2 id="protected-attributes-documentation" tabindex="-1">Protected Attributes Documentation <a class="header-anchor" href="#protected-attributes-documentation" aria-label="Permalink to &quot;Protected Attributes Documentation&quot;">​</a></h2><h3 id="variable-singlecolorid" tabindex="-1">variable singleColorId <a class="header-anchor" href="#variable-singlecolorid" aria-label="Permalink to &quot;variable singleColorId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint singleColorId;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint singleColorId;</span></span></code></pre></div><h3 id="variable-usesinglecolorid" tabindex="-1">variable useSingleColorId <a class="header-anchor" href="#variable-usesinglecolorid" aria-label="Permalink to &quot;variable useSingleColorId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint useSingleColorId;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint useSingleColorId;</span></span></code></pre></div><hr><p>Updated on 2024-10-16 at 16:58:00 +0000</p>`,52),l=[o];function n(p,i,c,d,h,u){return t(),a("div",null,l)}const f=e(r,[["render",n]]);export{m as __pageData,f as default};