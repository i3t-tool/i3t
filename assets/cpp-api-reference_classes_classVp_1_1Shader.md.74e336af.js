import{_ as s,o as a,c as e,Q as n}from"./chunks/framework.0f749972.js";const b=JSON.parse('{"title":"Vp::Shader","description":"","frontmatter":{"title":"Vp::Shader","summary":"Base class for shaders."},"headers":[],"relativePath":"cpp-api-reference/classes/classVp_1_1Shader.md","filePath":"cpp-api-reference/classes/classVp_1_1Shader.md"}'),t={name:"cpp-api-reference/classes/classVp_1_1Shader.md"},p=n(`<h1 id="vp-shader" tabindex="-1">Vp::Shader <a class="header-anchor" href="#vp-shader" aria-label="Permalink to &quot;Vp::Shader&quot;">​</a></h1><p>Base class for shaders. <a href="#detailed-description">More...</a></p><p><code>#include &quot;Viewport/shader/Shader.h&quot;</code></p><p>Inherited by <a href="/cpp-api-reference/classes/classvp_1_1boxblurshader.html">Vp::BoxBlurShader</a>, <a href="/cpp-api-reference/classes/classvp_1_1screenoverlayshader.html">Vp::ScreenOverlayShader</a>, <a href="/cpp-api-reference/classes/classvp_1_1selectioncompositeshader.html">Vp::SelectionCompositeShader</a>, <a href="/cpp-api-reference/classes/classvp_1_1wboitcompositeshader.html">Vp::WBOITCompositeShader</a>, <a href="/cpp-api-reference/classes/classvp_1_1worldshader.html">Vp::WorldShader</a></p><h2 id="public-functions" tabindex="-1">Public Functions <a class="header-anchor" href="#public-functions" aria-label="Permalink to &quot;Public Functions&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td></td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-shader">Shader</a></strong>(GLuint id)</td></tr><tr><td>virtual</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-~shader">~Shader</a></strong>() =default</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-bindtexture2d">bindTexture2D</a></strong>(GLuint textureUnit, GLuint textureID, GLint samplerLocation)</td></tr><tr><td>void</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-bindtexture2dms">bindTexture2DMS</a></strong>(GLuint textureUnit, GLuint textureID, GLint samplerLocation)</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-hasuniform">hasUniform</a></strong>(GLint location)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-init">init</a></strong>(bool initSuperclass)</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-setuniforms">setUniforms</a></strong>()&lt;br&gt;Set uniforms before rendering.</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-setuniformspermeshpart">setUniformsPerMeshPart</a></strong>(<a href="/cpp-api-reference/classes/structcore_1_1mesh_1_1meshpart.html">Core::Mesh::MeshPart</a> &amp; meshPart)&lt;br&gt;Set uniforms before rendering individual mesh parts.</td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-supportswboit">supportsWboit</a></strong>()</td></tr><tr><td>virtual void</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-use">use</a></strong>() const</td></tr></tbody></table><h2 id="public-attributes" tabindex="-1">Public Attributes <a class="header-anchor" href="#public-attributes" aria-label="Permalink to &quot;Public Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLuint</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#variable-m-id">m_id</a></strong></td></tr><tr><td>bool</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#variable-m-wboit">m_wboit</a></strong> &lt;br&gt;Whether to use weighted blended transparency for output if supported.</td></tr><tr><td>int</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#variable-m-wboitfunc">m_wboitFunc</a></strong></td></tr></tbody></table><h2 id="protected-attributes" tabindex="-1">Protected Attributes <a class="header-anchor" href="#protected-attributes" aria-label="Permalink to &quot;Protected Attributes&quot;">​</a></h2><table><thead><tr><th></th><th>Name</th></tr></thead><tbody><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#variable-m-wboitflagid">m_wboitFlagId</a></strong> &lt;br&gt;Uniform id of the wboit enable/disable flag.</td></tr><tr><td>GLint</td><td><strong><a href="/cpp-api-reference/classes/classvp_1_1shader.html#variable-m-wboitfuncid">m_wboitFuncId</a></strong></td></tr></tbody></table><h2 id="detailed-description" tabindex="-1">Detailed Description <a class="header-anchor" href="#detailed-description" aria-label="Permalink to &quot;Detailed Description&quot;">​</a></h2><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">class</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Vp</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">Shader</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">class</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Vp</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">Shader</span><span style="color:#24292E;">;</span></span></code></pre></div><p>Base class for shaders.</p><p>Note that shader objects are just a facade/API for the glsl shaders. They shouldn&#39;t store state across frames and should have ALL their variables and uniforms set before each use. However sometimes it makes sense to store values in them. Just be aware that shaders are shared between many objects.</p><h2 id="public-functions-documentation" tabindex="-1">Public Functions Documentation <a class="header-anchor" href="#public-functions-documentation" aria-label="Permalink to &quot;Public Functions Documentation&quot;">​</a></h2><h3 id="function-shader" tabindex="-1">function Shader <a class="header-anchor" href="#function-shader" aria-label="Permalink to &quot;function Shader&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">explicit</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">Shader</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    GLuint id</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">explicit</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">Shader</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    GLuint id</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-shader-1" tabindex="-1">function ~Shader <a class="header-anchor" href="#function-shader-1" aria-label="Permalink to &quot;function ~Shader&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">~</span><span style="color:#B392F0;">Shader</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">=default</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">~</span><span style="color:#6F42C1;">Shader</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">=default</span></span></code></pre></div><h3 id="function-bindtexture2d" tabindex="-1">function bindTexture2D <a class="header-anchor" href="#function-bindtexture2d" aria-label="Permalink to &quot;function bindTexture2D&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">bindTexture2D</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">GLuint</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">textureUnit</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">GLuint</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">textureID</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">GLint</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">samplerLocation</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">bindTexture2D</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">GLuint</span><span style="color:#24292E;"> </span><span style="color:#E36209;">textureUnit</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">GLuint</span><span style="color:#24292E;"> </span><span style="color:#E36209;">textureID</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">GLint</span><span style="color:#24292E;"> </span><span style="color:#E36209;">samplerLocation</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-bindtexture2dms" tabindex="-1">function bindTexture2DMS <a class="header-anchor" href="#function-bindtexture2dms" aria-label="Permalink to &quot;function bindTexture2DMS&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">bindTexture2DMS</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">GLuint</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">textureUnit</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">GLuint</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">textureID</span><span style="color:#E1E4E8;">,</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">GLint</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">samplerLocation</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">bindTexture2DMS</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">GLuint</span><span style="color:#24292E;"> </span><span style="color:#E36209;">textureUnit</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">GLuint</span><span style="color:#24292E;"> </span><span style="color:#E36209;">textureID</span><span style="color:#24292E;">,</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">GLint</span><span style="color:#24292E;"> </span><span style="color:#E36209;">samplerLocation</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-hasuniform" tabindex="-1">function hasUniform <a class="header-anchor" href="#function-hasuniform" aria-label="Permalink to &quot;function hasUniform&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">hasUniform</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">GLint</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">location</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">hasUniform</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">GLint</span><span style="color:#24292E;"> </span><span style="color:#E36209;">location</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><h3 id="function-init" tabindex="-1">function init <a class="header-anchor" href="#function-init" aria-label="Permalink to &quot;function init&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">init</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">initSuperclass</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">init</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#E36209;">initSuperclass</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classvp_1_1boxblurshader.html#function-init">Vp::BoxBlurShader::init</a>, <a href="/cpp-api-reference/classes/classvp_1_1colorshader.html#function-init">Vp::ColorShader::init</a>, <a href="/cpp-api-reference/classes/classvp_1_1frustumshader.html#function-init">Vp::FrustumShader::init</a>, <a href="/cpp-api-reference/classes/classvp_1_1gridshader.html#function-init">Vp::GridShader::init</a>, <a href="/cpp-api-reference/classes/classvp_1_1objectshader.html#function-init">Vp::ObjectShader::init</a>, <a href="/cpp-api-reference/classes/classvp_1_1phongshader.html#function-init">Vp::PhongShader::init</a>, <a href="/cpp-api-reference/classes/classvp_1_1screenoverlayshader.html#function-init">Vp::ScreenOverlayShader::init</a>, <a href="/cpp-api-reference/classes/classvp_1_1selectioncompositeshader.html#function-init">Vp::SelectionCompositeShader::init</a>, <a href="/cpp-api-reference/classes/classvp_1_1wboitcompositeshader.html#function-init">Vp::WBOITCompositeShader::init</a>, <a href="/cpp-api-reference/classes/classvp_1_1worldshader.html#function-init">Vp::WorldShader::init</a></p><h3 id="function-setuniforms" tabindex="-1">function setUniforms <a class="header-anchor" href="#function-setuniforms" aria-label="Permalink to &quot;function setUniforms&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setUniforms</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setUniforms</span><span style="color:#24292E;">()</span></span></code></pre></div><p>Set uniforms before rendering.</p><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classvp_1_1boxblurshader.html#function-setuniforms">Vp::BoxBlurShader::setUniforms</a>, <a href="/cpp-api-reference/classes/classvp_1_1colorshader.html#function-setuniforms">Vp::ColorShader::setUniforms</a>, <a href="/cpp-api-reference/classes/classvp_1_1frustumshader.html#function-setuniforms">Vp::FrustumShader::setUniforms</a>, <a href="/cpp-api-reference/classes/classvp_1_1gridshader.html#function-setuniforms">Vp::GridShader::setUniforms</a>, <a href="/cpp-api-reference/classes/classvp_1_1objectshader.html#function-setuniforms">Vp::ObjectShader::setUniforms</a>, <a href="/cpp-api-reference/classes/classvp_1_1phongshader.html#function-setuniforms">Vp::PhongShader::setUniforms</a>, <a href="/cpp-api-reference/classes/classvp_1_1screenoverlayshader.html#function-setuniforms">Vp::ScreenOverlayShader::setUniforms</a>, <a href="/cpp-api-reference/classes/classvp_1_1selectioncompositeshader.html#function-setuniforms">Vp::SelectionCompositeShader::setUniforms</a>, <a href="/cpp-api-reference/classes/classvp_1_1wboitcompositeshader.html#function-setuniforms">Vp::WBOITCompositeShader::setUniforms</a>, <a href="/cpp-api-reference/classes/classvp_1_1worldshader.html#function-setuniforms">Vp::WorldShader::setUniforms</a></p><h3 id="function-setuniformspermeshpart" tabindex="-1">function setUniformsPerMeshPart <a class="header-anchor" href="#function-setuniformspermeshpart" aria-label="Permalink to &quot;function setUniformsPerMeshPart&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">setUniformsPerMeshPart</span><span style="color:#E1E4E8;">(</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#B392F0;">Core</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">Mesh</span><span style="color:#E1E4E8;">::</span><span style="color:#B392F0;">MeshPart</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">&amp;</span><span style="color:#E1E4E8;"> </span><span style="color:#FFAB70;">meshPart</span></span>
<span class="line"><span style="color:#E1E4E8;">)</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">setUniformsPerMeshPart</span><span style="color:#24292E;">(</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#6F42C1;">Core</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">Mesh</span><span style="color:#24292E;">::</span><span style="color:#6F42C1;">MeshPart</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">&amp;</span><span style="color:#24292E;"> </span><span style="color:#E36209;">meshPart</span></span>
<span class="line"><span style="color:#24292E;">)</span></span></code></pre></div><p>Set uniforms before rendering individual mesh parts.</p><p><strong>Parameters</strong>:</p><ul><li><strong>meshPart</strong> MeshPart being rendered</li></ul><p><strong>Reimplemented by</strong>: <a href="/cpp-api-reference/classes/classvp_1_1phongshader.html#function-setuniformspermeshpart">Vp::PhongShader::setUniformsPerMeshPart</a></p><h3 id="function-supportswboit" tabindex="-1">function supportsWboit <a class="header-anchor" href="#function-supportswboit" aria-label="Permalink to &quot;function supportsWboit&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">supportsWboit</span><span style="color:#E1E4E8;">()</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">supportsWboit</span><span style="color:#24292E;">()</span></span></code></pre></div><p><strong>Return</strong>: Whether this shader supports output to wboit buffers</p><h3 id="function-use" tabindex="-1">function use <a class="header-anchor" href="#function-use" aria-label="Permalink to &quot;function use&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">virtual</span><span style="color:#E1E4E8;"> </span><span style="color:#F97583;">void</span><span style="color:#E1E4E8;"> </span><span style="color:#B392F0;">use</span><span style="color:#E1E4E8;">() </span><span style="color:#F97583;">const</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">virtual</span><span style="color:#24292E;"> </span><span style="color:#D73A49;">void</span><span style="color:#24292E;"> </span><span style="color:#6F42C1;">use</span><span style="color:#24292E;">() </span><span style="color:#D73A49;">const</span></span></code></pre></div><h2 id="public-attributes-documentation" tabindex="-1">Public Attributes Documentation <a class="header-anchor" href="#public-attributes-documentation" aria-label="Permalink to &quot;Public Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-id" tabindex="-1">variable m_id <a class="header-anchor" href="#variable-m-id" aria-label="Permalink to &quot;variable m_id&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLuint m_id;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLuint m_id;</span></span></code></pre></div><h3 id="variable-m-wboit" tabindex="-1">variable m_wboit <a class="header-anchor" href="#variable-m-wboit" aria-label="Permalink to &quot;variable m_wboit&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">bool</span><span style="color:#E1E4E8;"> m_wboit </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">false</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">bool</span><span style="color:#24292E;"> m_wboit </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">false</span><span style="color:#24292E;">;</span></span></code></pre></div><p>Whether to use weighted blended transparency for output if supported.</p><p><strong>See</strong>: <a href="/cpp-api-reference/classes/classvp_1_1shader.html#function-supportswboit">supportsWboit()</a></p><h3 id="variable-m-wboitfunc" tabindex="-1">variable m_wboitFunc <a class="header-anchor" href="#variable-m-wboitfunc" aria-label="Permalink to &quot;variable m_wboitFunc&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#F97583;">int</span><span style="color:#E1E4E8;"> m_wboitFunc </span><span style="color:#F97583;">=</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">0</span><span style="color:#E1E4E8;">;</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#D73A49;">int</span><span style="color:#24292E;"> m_wboitFunc </span><span style="color:#D73A49;">=</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">0</span><span style="color:#24292E;">;</span></span></code></pre></div><h2 id="protected-attributes-documentation" tabindex="-1">Protected Attributes Documentation <a class="header-anchor" href="#protected-attributes-documentation" aria-label="Permalink to &quot;Protected Attributes Documentation&quot;">​</a></h2><h3 id="variable-m-wboitflagid" tabindex="-1">variable m_wboitFlagId <a class="header-anchor" href="#variable-m-wboitflagid" aria-label="Permalink to &quot;variable m_wboitFlagId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint m_wboitFlagId {</span><span style="color:#F97583;">-</span><span style="color:#79B8FF;">1</span><span style="color:#E1E4E8;">};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint m_wboitFlagId {</span><span style="color:#D73A49;">-</span><span style="color:#005CC5;">1</span><span style="color:#24292E;">};</span></span></code></pre></div><p>Uniform id of the wboit enable/disable flag.</p><h3 id="variable-m-wboitfuncid" tabindex="-1">variable m_wboitFuncId <a class="header-anchor" href="#variable-m-wboitfuncid" aria-label="Permalink to &quot;variable m_wboitFuncId&quot;">​</a></h3><div class="language-cpp vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">cpp</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#E1E4E8;">GLint m_wboitFuncId {</span><span style="color:#F97583;">-</span><span style="color:#79B8FF;">1</span><span style="color:#E1E4E8;">};</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292E;">GLint m_wboitFuncId {</span><span style="color:#D73A49;">-</span><span style="color:#005CC5;">1</span><span style="color:#24292E;">};</span></span></code></pre></div><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>`,60),o=[p];function l(r,c,i,d,h,u){return a(),e("div",null,o)}const E=s(t,[["render",l]]);export{b as __pageData,E as default};
