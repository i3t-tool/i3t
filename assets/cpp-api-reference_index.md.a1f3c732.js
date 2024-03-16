import{_ as e,o as a,c as t,Q as o}from"./chunks/framework.0f749972.js";const m=JSON.parse('{"title":"Documentation before refactoring","description":"","frontmatter":{"title":"Documentation before refactoring","summary":"I3T is an educational application which enables the study of 3D transformations and their hierarchy in an illustrative way."},"headers":[],"relativePath":"cpp-api-reference/index.md","filePath":"cpp-api-reference/index.md"}'),n={name:"cpp-api-reference/index.md"},i=o('<h1 id="documentation-before-refactoring" tabindex="-1">Documentation before refactoring <a class="header-anchor" href="#documentation-before-refactoring" aria-label="Permalink to &quot;Documentation before refactoring&quot;">​</a></h1><p><a href="/cpp-api-reference/namespaces/namespacei3t.html">I3T</a> is an educational application which enables the study of 3D transformations and their hierarchy in an illustrative way. This tool is a result of master&#39;s thesis by Michal Folta done in the year 2016 at the <a href="http://dcgi.fel.cvut.cz/" target="_blank" rel="noreferrer">Department of Computer Graphics and Interaction</a>, FEL, CTU in Prague. The development continues... The tool homepage: <a href="http://www.i3t-tool.org/" target="_blank" rel="noreferrer">http://www.i3t-tool.org/</a>.</p><h1 id="the-layout-of-the-gui" tabindex="-1">The layout of the GUI <a class="header-anchor" href="#the-layout-of-the-gui" aria-label="Permalink to &quot;The layout of the GUI&quot;">​</a></h1><p>The scene is divided into two parts - the 3D scene preview and a workspace with the scene graph boxes.</p><ul><li>3D scene is defined in the class World and rendered using the class Scene.</li><li>The workspace is stored in tabGroup.layers[1].tabs. TabGroup is the root of all <a href="/cpp-api-reference/namespaces/namespacegui.html">GUI</a> objects.</li></ul><h1 id="engine" tabindex="-1">Engine <a class="header-anchor" href="#engine" aria-label="Permalink to &quot;Engine&quot;">​</a></h1><h1 id="logic" tabindex="-1">Logic <a class="header-anchor" href="#logic" aria-label="Permalink to &quot;Logic&quot;">​</a></h1><h1 id="gui" tabindex="-1">GUI <a class="header-anchor" href="#gui" aria-label="Permalink to &quot;GUI&quot;">​</a></h1><ul><li>base components: button, form, textTab, checkBox,... They are created independently on the rest of the program</li><li>specialized components for <a href="/cpp-api-reference/namespaces/namespacei3t.html">I3T</a> tool: The base class is the class <em>Tab</em>. This class serves as a base class for all <a href="/cpp-api-reference/namespaces/namespacegui.html">GUI</a> elements, such as boxes on the screen, such as Forms (boxes), operators, labels, check-boxes, numerical edit fields, ... The colors, sizes, and state of <a href="/cpp-api-reference/namespaces/namespacegui.html">GUI</a> components (and also the colors of the wires) and the key bindings are defined in a static structure <em>TabsConfig</em>.</li></ul><h1 id="main-cpp" tabindex="-1">main.cpp <a class="header-anchor" href="#main-cpp" aria-label="Permalink to &quot;main.cpp&quot;">​</a></h1><p>Main defines the World - update() handles all interaction.</p><h1 id="external-documentation" tabindex="-1">External documentation <a class="header-anchor" href="#external-documentation" aria-label="Permalink to &quot;External documentation&quot;">​</a></h1><p>Files with models can be either in Wavefront <a href="https://en.wikipedia.org/wiki/Wavefront_.obj_file" target="_blank" rel="noreferrer">.obj</a> format or in a proprietary .tmsh format.</p><h1 id="notes-not-to-forget-about" tabindex="-1">Notes not to forget about <a class="header-anchor" href="#notes-not-to-forget-about" aria-label="Permalink to &quot;Notes not to forget about&quot;">​</a></h1><ul><li><a href="./todo.html">todolist </a></li><li>Properties <ul><li>REMOVED: C++/Advanced: Disable specific warnings: 4482;%(DisableSpecificWarnings) Qualifying an enumeration value with the name of the enumeration is not valid C++03, it is valid C++11 - it was a problem for VS2010</li><li>Enable incremental linking: YES</li><li>Edit and continue &lt;A href=&quot;<a href="https://docs.microsoft.com/en-us/visualstudio/debugger/edit-and-continue?view=vs-2019" target="_blank" rel="noreferrer">https://docs.microsoft.com/en-us/visualstudio/debugger/edit-and-continue?view=vs-2019</a>&gt;Documentation</li></ul></li></ul><p>Tools &gt; Options &gt; (or Debug &gt; Options) &gt; Debugging &gt; General, select Edit and Continue in the right pane. * Apply changes on continue (Native only) If selected, Visual Studio automatically compiles and applies code changes when you continue debugging from a break state. Otherwise, you can choose to apply changes using Debug &gt; Apply Code Changes. * Warn about stale code (Native only) If selected, gives warnings about stale code.</p><hr><p>Updated on 2024-03-16 at 18:59:45 +0000</p>',18),r=[i];function s(l,c,h,d,p,u){return a(),t("div",null,r)}const g=e(n,[["render",s]]);export{m as __pageData,g as default};
