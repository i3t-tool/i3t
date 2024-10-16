import{_ as e,o as a,c as t,Q as s}from"./chunks/framework.0f749972.js";const m=JSON.parse('{"title":"Vp::BoxBlurShader","description":"","frontmatter":{"title":"Vp::BoxBlurShader","summary":"Two pass box blur with alpha support."},"headers":[],"relativePath":"cpp-api-reference/classes/classVp_1_1BoxBlurShader.md","filePath":"cpp-api-reference/classes/classVp_1_1BoxBlurShader.md"}'),r={name:"cpp-api-reference/classes/classVp_1_1BoxBlurShader.md"},l=s(`<h1 id="vp-boxblurshader" tabindex="-1">Vp::BoxBlurShader <a class="header-anchor" href="#vp-boxblurshader" aria-label="Permalink to &quot;Vp::BoxBlurShader&quot;">​</a></h1><p>Two pass box blur with alpha support.</p><p><code>#include &quot;Viewport/shader/BoxBlurShader.h&quot;</code></p><p>Inherits from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#function-boxblurshader">BoxBlurShader</a></strong>(GLuint id)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#function-init">init</a></strong>(bool initSuperclass) override</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#function-setuniforms">setUniforms</a></strong>() override&lt;br&gt;Set uniforms before rendering.</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>int</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#variable-m-kernelsize">m_kernelSize</a></strong> &lt;br&gt;Size of the blur kernel in pixels.</td></tr><tr><td>glm::vec2</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#variable-m-resolution">m_resolution</a></strong> &lt;br&gt;Resolution of the current buffer in pixels.</td></tr><tr><td>GLuint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#variable-m-sourcetextureid">m_sourceTextureId</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#variable-m-vertical">m_vertical</a></strong> &lt;br&gt;Vertical or horizontal blur pass.</td></tr></tbody></table><h2 id="protected-attributes" tabindex="-1">Protected Attributes <a class="header-anchor" href="#protected-attributes" aria-label="Permalink to &quot;Protected Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#variable-m-kernelsizeid">m_kernelSizeId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#variable-m-resolutionid">m_resolutionId</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#variable-m-sourcesampler">m_sourceSampler</a></strong></td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1BoxBlurShader.html#variable-m-verticalid">m_verticalId</a></strong></td></tr></tbody></table><h2 id="additional-inherited-members" tabindex="-1">Additional inherited members <a class="header-anchor" href="#additional-inherited-members" aria-label="Permalink to &quot;Additional inherited members&quot;">​</a></h2><p><strong>Public Functions inherited from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-shader">Shader</a></strong>(GLuint id)</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-~shader">~Shader</a></strong>() =default</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-bindtexture2d">bindTexture2D</a></strong>(GLuint textureUnit, GLuint textureID, GLint samplerLocation)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-bindtexture2dms">bindTexture2DMS</a></strong>(GLuint textureUnit, GLuint textureID, GLint samplerLocation)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-hasuniform">hasUniform</a></strong>(GLint location)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-setuniformspermeshpart">setUniformsPerMeshPart</a></strong>(<a href="/cpp-api-reference/classes/structCore_1_1Mesh_1_1MeshPart.html">Core::Mesh::MeshPart</a> &amp; meshPart)&lt;br&gt;Set uniforms before rendering individual mesh parts.</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-supportswboit">supportsWboit</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-use">use</a></strong>() const</td></tr></tbody></table><p><strong>Public Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLuint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-id">m_id</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboit">m_wboit</a></strong> &lt;br&gt;Whether to use weighted blended transparency for output if supported.</td></tr><tr><td>int</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboitfunc">m_wboitFunc</a></strong></td></tr></tbody></table><p><strong>Protected Attributes inherited from <a href="/cpp-api-reference/classes/classVp_1_1Shader.html">Vp::Shader</a></strong></p><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboitflagid">m_wboitFlagId</a></strong> &lt;br&gt;Uniform id of the wboit enable/disable flag.</td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classVp_1_1Shader.html#variable-m-wboitfuncid">m_wboitFuncId</a></strong></td></tr></tbody></table><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-boxblurshader" tabindex="-1">function BoxBlurShader <a class="header-anchor" href="#function-boxblurshader" aria-label="Permalink to &quot;function BoxBlurShader&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">explicit</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">BoxBlurShader</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    GLuint id</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">explicit</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">BoxBlurShader</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    GLuint id</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-init" tabindex="-1">function init <a class="header-anchor" href="#function-init" aria-label="Permalink to &quot;function init&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">init</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">initSuperclass</span></span>
<span class="line"><span style="color:#E1E4E8;">) </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">init</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#E36209;">initSuperclass</span></span>
<span class="line"><span style="color:#24292E;">) </span><span style="color:#D73A49;">override</span></span></code></pre></div><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-init">Vp::Shader::init</a></p><h3 id="function-setuniforms" tabindex="-1">function setUniforms <a class="header-anchor" href="#function-setuniforms" aria-label="Permalink to &quot;function setUniforms&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setUniforms</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">override</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setUniforms</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">override</span></span></code></pre></div><p>Set uniforms before rendering.</p><p><strong>Reimplements</strong>: <a href="/cpp-api-reference/classes/classVp_1_1Shader.html#function-setuniforms">Vp::Shader::setUniforms</a></p><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-kernelsize" tabindex="-1">variable m_kernelSize <a class="header-anchor" href="#variable-m-kernelsize" aria-label="Permalink to &quot;variable m_kernelSize&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">int</span><span style="color:#E1E4E8;"> m_kernelSize {</span><span style="color:#79B8FF;">10</span><span style="color:#E1E4E8;">};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">int</span><span style="color:#24292E;"> m_kernelSize {</span><span style="color:#005CC5;">10</span><span style="color:#24292E;">};</span></span></code></pre></div><p>Size of the blur kernel in pixels.</p><h3 id="variable-m-resolution" tabindex="-1">variable m_resolution <a class="header-anchor" href="#variable-m-resolution" aria-label="Permalink to &quot;variable m_resolution&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">glm</span><span style="color:#E1E4E8;">::vec2 m_resolution;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">glm</span><span style="color:#24292E;">::vec2 m_resolution;</span></span></code></pre></div><p>Resolution of the current buffer in pixels.</p><h3 id="variable-m-sourcetextureid" tabindex="-1">variable m_sourceTextureId <a class="header-anchor" href="#variable-m-sourcetextureid" aria-label="Permalink to &quot;variable m_sourceTextureId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLuint m_sourceTextureId {</span><span style="color:#79B8FF;">0</span><span style="color:#E1E4E8;">};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLuint m_sourceTextureId {</span><span style="color:#005CC5;">0</span><span style="color:#24292E;">};</span></span></code></pre></div><h3 id="variable-m-vertical" tabindex="-1">variable m_vertical <a class="header-anchor" href="#variable-m-vertical" aria-label="Permalink to &quot;variable m_vertical&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> m_vertical {</span><span style="color:#79B8FF;">true</span><span style="color:#E1E4E8;">};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> m_vertical {</span><span style="color:#005CC5;">true</span><span style="color:#24292E;">};</span></span></code></pre></div><p>Vertical or horizontal blur pass.</p><h2 id="protected-attributes-documentation" tabindex="-1">Protected Attributes Documentation <a class="header-anchor" href="#protected-attributes-documentation" aria-label="Permalink to &quot;Protected Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-kernelsizeid" tabindex="-1">variable m_kernelSizeId <a class="header-anchor" href="#variable-m-kernelsizeid" aria-label="Permalink to &quot;variable m_kernelSizeId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint m_kernelSizeId;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint m_kernelSizeId;</span></span></code></pre></div><h3 id="variable-m-resolutionid" tabindex="-1">variable m_resolutionId <a class="header-anchor" href="#variable-m-resolutionid" aria-label="Permalink to &quot;variable m_resolutionId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint m_resolutionId;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint m_resolutionId;</span></span></code></pre></div><h3 id="variable-m-sourcesampler" tabindex="-1">variable m_sourceSampler <a class="header-anchor" href="#variable-m-sourcesampler" aria-label="Permalink to &quot;variable m_sourceSampler&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint m_sourceSampler;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint m_sourceSampler;</span></span></code></pre></div><h3 id="variable-m-verticalid" tabindex="-1">variable m_verticalId <a class="header-anchor" href="#variable-m-verticalid" aria-label="Permalink to &quot;variable m_verticalId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint m_verticalId;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint m_verticalId;</span></span></code></pre></div><hr><p>Updated on 2024-10-16 at 16:58:00 +0000</p>`,50),n=[l];function o(i,p,c,d,h,u){return a(),t("div",null,n)}const f=e(r,[["render",o]]);export{m as __pageData,f as default};