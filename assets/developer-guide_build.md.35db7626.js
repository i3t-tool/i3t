import{_ as s,o as a,c as e,Q as n}from"./chunks/framework.0f749972.js";const h=JSON.parse('{"title":"Build","description":"","frontmatter":{},"headers":[],"relativePath":"developer-guide/build.md","filePath":"developer-guide/build.md"}'),o={name:"developer-guide/build.md"},l=n(`<h1 id="build" tabindex="-1">Build <a class="header-anchor" href="#build" aria-label="Permalink to &quot;Build&quot;">​</a></h1><p>List of supported platforms is in <a href="https://github.com/i3t-tool/i3t/blob/master/README.md#Build" target="_blank" rel="noreferrer">table</a> in README.md file.</p><h2 id="get-source-code" tabindex="-1">Get source code <a class="header-anchor" href="#get-source-code" aria-label="Permalink to &quot;Get source code&quot;">​</a></h2><p>First, you need to download the source code. The <code>--recursive</code> switch ensures that git submodules with libraries will be downloaded together with the main repository.</p><div class="language-shell vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">shell</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">git</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">clone</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">--recursive</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">https://github.com/i3t-tool/i3t.git</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">git</span><span style="color:#24292E;"> </span><span style="color:#032F62;">clone</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">--recursive</span><span style="color:#24292E;"> </span><span style="color:#032F62;">https://github.com/i3t-tool/i3t.git</span></span></code></pre></div><p>If not all submodules were initialized or if any was removed in a new version, they must be reinitialized.</p><div class="language-shell vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">shell</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">git</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">submodule</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">update</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">--init</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">--recursive</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">git</span><span style="color:#24292E;"> </span><span style="color:#032F62;">submodule</span><span style="color:#24292E;"> </span><span style="color:#032F62;">update</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">--init</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">--recursive</span></span></code></pre></div><h2 id="dependencies" tabindex="-1">Dependencies <a class="header-anchor" href="#dependencies" aria-label="Permalink to &quot;Dependencies&quot;">​</a></h2><p>You will need <code>cmake</code> and a C++23 compiler with support for <code>std::expected</code>.</p><h3 id="linux-apt" tabindex="-1">Linux (apt) <a class="header-anchor" href="#linux-apt" aria-label="Permalink to &quot;Linux (apt)&quot;">​</a></h3><div class="language-shell vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">shell</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">apt</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">install</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">-y</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">libgl-dev</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">libglx-dev</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">\\</span></span>
<span class="line"><span style="color:#E1E4E8;">    </span><span style="color:#9ECBFF;">libxrandr-dev</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">libxinerama-dev</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">libxcursor-dev</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">libxi-dev</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">apt</span><span style="color:#24292E;"> </span><span style="color:#032F62;">install</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">-y</span><span style="color:#24292E;"> </span><span style="color:#032F62;">libgl-dev</span><span style="color:#24292E;"> </span><span style="color:#032F62;">libglx-dev</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">\\</span></span>
<span class="line"><span style="color:#24292E;">    </span><span style="color:#032F62;">libxrandr-dev</span><span style="color:#24292E;"> </span><span style="color:#032F62;">libxinerama-dev</span><span style="color:#24292E;"> </span><span style="color:#032F62;">libxcursor-dev</span><span style="color:#24292E;"> </span><span style="color:#032F62;">libxi-dev</span></span></code></pre></div><h2 id="cmake" tabindex="-1">CMake <a class="header-anchor" href="#cmake" aria-label="Permalink to &quot;CMake&quot;">​</a></h2><p>Project build is realized by <code>cmake</code> utility. All necessary dependencies are contained in the <code>Dependencies/</code> directory.</p><p>The directory in which CMake is run is taken as the output directory, into which files for build systems (Makefile, *.vxproj, ...) are generated.</p><p><strong>Code:</strong> Generating configuration using cmake. <strong>To build a distributable binary, you need to specify the variable <code>CMAKE_BUILD_TYPE=Release</code>.</strong></p><div class="language-shell vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">shell</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#6A737D;"># In the project root create cmake output directory &#39;out/&#39; and move into it.</span></span>
<span class="line"><span style="color:#B392F0;">mkdir</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">out</span><span style="color:#E1E4E8;"> &amp;&amp; </span><span style="color:#79B8FF;">cd</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">out</span></span>
<span class="line"></span>
<span class="line"><span style="color:#6A737D;"># Build in the directory MSVC project files.</span></span>
<span class="line"><span style="color:#B392F0;">cmake</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">-DCMAKE_BUILD_TYPE=Debug</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">-A</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">Win32</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">-G</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;Visual Studio 16 2019&quot;</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">..</span></span>
<span class="line"><span style="color:#6A737D;"># or for Code-Blocks projekt on Linux</span></span>
<span class="line"><span style="color:#B392F0;">cmake</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">-DCMAKE_BUILD_TYPE=Debug</span><span style="color:#E1E4E8;"> </span><span style="color:#79B8FF;">-G</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">&quot;CodeBlocks - Unix Makefiles&quot;</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">..</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6A737D;"># In the project root create cmake output directory &#39;out/&#39; and move into it.</span></span>
<span class="line"><span style="color:#6F42C1;">mkdir</span><span style="color:#24292E;"> </span><span style="color:#032F62;">out</span><span style="color:#24292E;"> &amp;&amp; </span><span style="color:#005CC5;">cd</span><span style="color:#24292E;"> </span><span style="color:#032F62;">out</span></span>
<span class="line"></span>
<span class="line"><span style="color:#6A737D;"># Build in the directory MSVC project files.</span></span>
<span class="line"><span style="color:#6F42C1;">cmake</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">-DCMAKE_BUILD_TYPE=Debug</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">-A</span><span style="color:#24292E;"> </span><span style="color:#032F62;">Win32</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">-G</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;Visual Studio 16 2019&quot;</span><span style="color:#24292E;"> </span><span style="color:#032F62;">..</span></span>
<span class="line"><span style="color:#6A737D;"># or for Code-Blocks projekt on Linux</span></span>
<span class="line"><span style="color:#6F42C1;">cmake</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">-DCMAKE_BUILD_TYPE=Debug</span><span style="color:#24292E;"> </span><span style="color:#005CC5;">-G</span><span style="color:#24292E;"> </span><span style="color:#032F62;">&quot;CodeBlocks - Unix Makefiles&quot;</span><span style="color:#24292E;"> </span><span style="color:#032F62;">..</span></span></code></pre></div><p>This example of running CMake from the command line generates project files for Visual Studio 2019 in the directory where CMake is run, for the <code>Debug | x86</code> configuration. Two dots indicate the path to the main file <code>CMakeLists.txt</code>, which is located in the root</p><p>The resulting binary files, shared libraries and the IT3(.exe) executable are created in the <code>Binaries</code> directory into a subdirectory corresponding to the configuration type.</p><h2 id="bundle" tabindex="-1">Bundle <a class="header-anchor" href="#bundle" aria-label="Permalink to &quot;Bundle&quot;">​</a></h2><p>You can create a zip archive containing the ready-to-use I3T and its data, but first, you need to build the I3T.</p><div class="language-shell vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang">shell</span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#B392F0;">Scripts/CreateBundle.sh</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">Debug</span><span style="color:#E1E4E8;">  </span><span style="color:#6A737D;"># or</span></span>
<span class="line"><span style="color:#B392F0;">Scripts/CreateBundle.sh</span><span style="color:#E1E4E8;"> </span><span style="color:#9ECBFF;">Release</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#6F42C1;">Scripts/CreateBundle.sh</span><span style="color:#24292E;"> </span><span style="color:#032F62;">Debug</span><span style="color:#24292E;">  </span><span style="color:#6A737D;"># or</span></span>
<span class="line"><span style="color:#6F42C1;">Scripts/CreateBundle.sh</span><span style="color:#24292E;"> </span><span style="color:#032F62;">Release</span></span></code></pre></div>`,21),p=[l];function t(c,r,i,d,y,E){return a(),e("div",null,p)}const F=s(o,[["render",t]]);export{h as __pageData,F as default};
