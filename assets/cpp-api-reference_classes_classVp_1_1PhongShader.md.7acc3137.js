import{_ as a,o as s,c as e,Q as t}from"./chunks/framework.0f749972.js";const b=JSON.parse('{"title":"Vp::PhongShader","description":"","frontmatter":{"title":"Vp::PhongShader","summary":"Shader for lit objects with textures."},"headers":[],"relativePath":"cpp-api-reference/classes/classVp_1_1PhongShader.md","filePath":"cpp-api-reference/classes/classVp_1_1PhongShader.md"}'),n={name:"cpp-api-reference/classes/classVp_1_1PhongShader.md"},r=t(`<h1 id="vp-phongshader" tabindex="-1">Vp::PhongShader <a class="header-anchor" href="#vp-phongshader" aria-label="Permalink to &quot;Vp::PhongShader&quot;">​</a></h1><p><a href="./.html">Shader</a> for lit objects with textures.</p><p><code>#include &quot;Viewport/shader/PhongShader.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html">Vp::ObjectShader</a>, <a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html">Vp::WorldShader</a>, <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></p><h2 id="public-types" tabindex="-1">Public Types <a class="header-anchor" href="#public-types" aria-label="Permalink to &quot;Public Types&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>enum</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#enum-lightingmodel">LightingModel</a></strong> { PHONG, BLINN_PHONG}</td></tr></tbody></table><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#function-phongshader">PhongShader</a></strong>(GLuint id)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#function-init">init</a></strong>(bool initSuperclass) override</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#function-setuniforms">setUniforms</a></strong>() override&lt;br&gt;Set uniforms before rendering.</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#function-setuniformspermeshpart">setUniformsPerMeshPart</a></strong>(<a href="/cpp-api-reference/classes/structCore_1_1Mesh_1_1MeshPart.html">Core::Mesh::MeshPart</a> &amp; meshPart) override&lt;br&gt;Set uniforms before rendering individual mesh parts.</td></tr></tbody></table><h2 id="protected-functions" tabindex="-1">Protected Functions <a class="header-anchor" href="#protected-functions" aria-label="Permalink to &quot;Protected Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#function-bindtexture">bindTexture</a></strong>(GLuint textureID, const std::string &amp; type, int typeIndex, GLuint textureUnit)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#function-bindtextures">bindTextures</a></strong>(<a href="/cpp-api-reference/classes/structCore_1_1Mesh_1_1TextureSet.html">Core::Mesh::TextureSet</a> tSet)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#function-cleartextures">clearTextures</a></strong>() const&lt;br&gt;Sets all texture active flags to false.</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#function-setmaterialuniforms">setMaterialUniforms</a></strong>(<a href="/cpp-api-reference/classes/structCore_1_1Mesh_1_1Material.html">Core::Mesh::Material</a> material)</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#enum-lightingmodel">LightingModel</a></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#variable-m-lightingmodel">m_lightingModel</a></strong></td></tr><tr><td>glm::vec3</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#variable-m-tint">m_tint</a></strong></td></tr></tbody></table><h2 id="protected-attributes" tabindex="-1">Protected Attributes <a class="header-anchor" href="#protected-attributes" aria-label="Permalink to &quot;Protected Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#variable-m-lightingmodelid">m_lightingModelId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#variable-material-ambient">material_ambient</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#variable-material-diffuse">material_diffuse</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#variable-material-shininess">material_shininess</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#variable-material-specular">material_specular</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#variable-normalstrength">normalStrength</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1PhongShader.html#variable-tintid">tintId</a></strong></td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html">Vp::ObjectShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html#function-objectshader">ObjectShader</a></strong>(GLuint id)</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html">Vp::ObjectShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>float</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html#variable-m-opacity">m_opacity</a></strong></td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html">Vp::ObjectShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html#variable-opacityid">opacityId</a></strong></td></tr></tbody></table><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html">Vp::WorldShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#function-worldshader">WorldShader</a></strong>(GLuint id)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#function-setworldtransform">setWorldTransform</a></strong>(const glm::mat4 &amp; model, const glm::mat4 &amp; view, const glm::mat4 &amp; projection)&lt;br&gt;Updates MVP matrices.</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html">Vp::WorldShader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>glm::mat4</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-model">m_model</a></strong></td></tr><tr><td>glm::mat4</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-projection">m_projection</a></strong></td></tr><tr><td>glm::mat4</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-view">m_view</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-wboitfarid">m_wboitFarId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-m-wboitnearid">m_wboitNearId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-modelmatrixid">modelMatrixId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-normalmatrixid">normalMatrixId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-projectionmatrixid">projectionMatrixId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-pvmmatrixid">pvmMatrixId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1WorldShader.html#variable-viewmatrixid">viewMatrixId</a></strong></td></tr></tbody></table><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-shader">Shader</a></strong>(GLuint id)</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-~shader">~Shader</a></strong>() =default</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-bindtexture2d">bindTexture2D</a></strong>(GLuint textureUnit, GLuint textureID, GLint samplerLocation)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-bindtexture2dms">bindTexture2DMS</a></strong>(GLuint textureUnit, GLuint textureID, GLint samplerLocation)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-hasuniform">hasUniform</a></strong>(GLint location)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-supportswboit">supportsWboit</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-use">use</a></strong>() const</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLuint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-id">m_id</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboit">m_wboit</a></strong> &lt;br&gt;Whether to use weighted blended transparency for output if supported.</td></tr><tr><td>int</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboitfunc">m_wboitFunc</a></strong></td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboitflagid">m_wboitFlagId</a></strong> &lt;br&gt;Uniform id of the wboit enable/disable flag.</td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboitfuncid">m_wboitFuncId</a></strong></td></tr></tbody></table><h2 id="public-types-documentation" tabindex="-1">Public Types Documentation <a class="header-anchor" href="#public-types-documentation" aria-label="Permalink to &quot;Public Types Documentation&quot;">​</a></h2><h3 id="enum-lightingmodel" tabindex="-1">enum LightingModel <a class="header-anchor" href="#enum-lightingmodel" aria-label="Permalink to &quot;enum LightingModel&quot;">​</a></h3><table><thead><tr><th>Enumerator</th><th>Value</th><th>Description</th></tr></thead><tbody><tr><td>PHONG</td><td></td><td></td></tr><tr><td>BLINN_PHONG</td><td></td><td></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-phongshader" tabindex="-1">function PhongShader <a class="header-anchor" href="#function-phongshader" aria-label="Permalink to &quot;function PhongShader&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">explicit</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">PhongShader</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    GLuint id</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">explicit</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">PhongShader</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    GLuint id</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-init" tabindex="-1">function init <a class="header-anchor" href="#function-init" aria-label="Permalink to &quot;function init&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">init</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">initSuperclass</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">init</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#E36209;">initSuperclass</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">override</span></span></code></pre></div><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html#function-init">Vp::ObjectShader::init</a></p><h3 id="function-setuniforms" tabindex="-1">function setUniforms <a class="header-anchor" href="#function-setuniforms" aria-label="Permalink to &quot;function setUniforms&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setUniforms</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setUniforms</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">override</span></span></code></pre></div><p>Set uniforms before rendering.</p><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classVp_1_1ObjectShader.html#function-setuniforms">Vp::ObjectShader::setUniforms</a></p><h3 id="function-setuniformspermeshpart" tabindex="-1">function setUniformsPerMeshPart <a class="header-anchor" href="#function-setuniformspermeshpart" aria-label="Permalink to &quot;function setUniformsPerMeshPart&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setUniformsPerMeshPart</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Core</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">Mesh</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">MeshPart</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">meshPart</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setUniformsPerMeshPart</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Core</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">Mesh</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">MeshPart</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">meshPart</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">override</span></span></code></pre></div><p>Set uniforms before rendering individual mesh parts.</p><p><strong>Parameters</strong>:</p><ul><li><strong>meshPart</strong> MeshPart being rendered</li></ul><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-setuniformspermeshpart">Vp::Shader::setUniformsPerMeshPart</a></p><h2 id="protected-functions-documentation" tabindex="-1">Protected Functions Documentation <a class="header-anchor" href="#protected-functions-documentation" aria-label="Permalink to &quot;Protected Functions Documentation&quot;">​</a></h2><h3 id="function-bindtexture" tabindex="-1">function bindTexture <a class="header-anchor" href="#function-bindtexture" aria-label="Permalink to &quot;function bindTexture&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">bindTexture</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">GLuint</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">textureID</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">const</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">std</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">string</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">type</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">int</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">typeIndex</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">GLuint</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">textureUnit</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">bindTexture</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">GLuint</span><span style="color:#24292E;"> </span><span style="color:#E36209;">textureID</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">const</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">std</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">string</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">type</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">int</span><span style="color:#24292E;"> </span><span style="color:#E36209;">typeIndex</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">GLuint</span><span style="color:#24292E;"> </span><span style="color:#E36209;">textureUnit</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-bindtextures" tabindex="-1">function bindTextures <a class="header-anchor" href="#function-bindtextures" aria-label="Permalink to &quot;function bindTextures&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">bindTextures</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Core</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">Mesh</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">TextureSet</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">tSet</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">bindTextures</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Core</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">Mesh</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">TextureSet</span><span style="color:#24292E;"> </span><span style="color:#E36209;">tSet</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-cleartextures" tabindex="-1">function clearTextures <a class="header-anchor" href="#function-cleartextures" aria-label="Permalink to &quot;function clearTextures&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">clearTextures</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">clearTextures</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><p>Sets all texture active flags to false.</p><p>Used to reset texture state before binding new set of textures.</p><h3 id="function-setmaterialuniforms" tabindex="-1">function setMaterialUniforms <a class="header-anchor" href="#function-setmaterialuniforms" aria-label="Permalink to &quot;function setMaterialUniforms&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setMaterialUniforms</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Core</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">Mesh</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">Material</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">material</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setMaterialUniforms</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Core</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">Mesh</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">Material</span><span style="color:#24292E;"> </span><span style="color:#E36209;">material</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-lightingmodel" tabindex="-1">variable m_lightingModel <a class="header-anchor" href="#variable-m-lightingmodel" aria-label="Permalink to &quot;variable m_lightingModel&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">LightingModel m_lightingModel {BLINN_PHONG};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">LightingModel m_lightingModel {BLINN_PHONG};</span></span></code></pre></div><h3 id="variable-m-tint" tabindex="-1">variable m_tint <a class="header-anchor" href="#variable-m-tint" aria-label="Permalink to &quot;variable m_tint&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::vec3 m_tint {</span><span style="color:#79B8FF;">1.0</span><span style="color:#F97583;">f</span><span style="color:#E1E4E8;">};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::vec3 m_tint {</span><span style="color:#005CC5;">1.0</span><span style="color:#D73A49;">f</span><span style="color:#24292E;">};</span></span></code></pre></div><h2 id="protected-attributes-documentation" tabindex="-1">Protected Attributes Documentation <a class="header-anchor" href="#protected-attributes-documentation" aria-label="Permalink to &quot;Protected Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-lightingmodelid" tabindex="-1">variable m_lightingModelId <a class="header-anchor" href="#variable-m-lightingmodelid" aria-label="Permalink to &quot;variable m_lightingModelId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint m_lightingModelId;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint m_lightingModelId;</span></span></code></pre></div><h3 id="variable-material-ambient" tabindex="-1">variable material_ambient <a class="header-anchor" href="#variable-material-ambient" aria-label="Permalink to &quot;variable material_ambient&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint material_ambient;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint material_ambient;</span></span></code></pre></div><h3 id="variable-material-diffuse" tabindex="-1">variable material_diffuse <a class="header-anchor" href="#variable-material-diffuse" aria-label="Permalink to &quot;variable material_diffuse&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark has-diff vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint material_diffuse;</span></span></code></pre><pre class="shiki github-light has-diff vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint material_diffuse;</span></span></code></pre></div><h3 id="variable-material-shininess" tabindex="-1">variable material_shininess <a class="header-anchor" href="#variable-material-shininess" aria-label="Permalink to &quot;variable material_shininess&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint material_shininess;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint material_shininess;</span></span></code></pre></div><h3 id="variable-material-specular" tabindex="-1">variable material_specular <a class="header-anchor" href="#variable-material-specular" aria-label="Permalink to &quot;variable material_specular&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint material_specular;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint material_specular;</span></span></code></pre></div><h3 id="variable-normalstrength" tabindex="-1">variable normalStrength <a class="header-anchor" href="#variable-normalstrength" aria-label="Permalink to &quot;variable normalStrength&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint normalStrength;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint normalStrength;</span></span></code></pre></div><h3 id="variable-tintid" tabindex="-1">variable tintId <a class="header-anchor" href="#variable-tintid" aria-label="Permalink to &quot;variable tintId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint tintId;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint tintId;</span></span></code></pre></div><hr><p>Updated on 2024-10-16 at 16:44:50 +0000</p>`,83),l=[r];function o(p,i,c,d,h,u){return s(),e("div",null,l)}const g=a(n,[["render",o]]);export{b as __pageData,g as default};