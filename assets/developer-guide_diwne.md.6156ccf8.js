import{_ as e,o,c as i,Q as n}from"./chunks/framework.0f749972.js";const w=JSON.parse('{"title":"DIWNE - Dear Imgui Wrapper Node Editor","description":"","frontmatter":{},"headers":[],"relativePath":"developer-guide/diwne.md","filePath":"developer-guide/diwne.md"}'),a={name:"developer-guide/diwne.md"},t=n(`<h1 id="diwne-dear-imgui-wrapper-node-editor" tabindex="-1">DIWNE - Dear Imgui Wrapper Node Editor <a class="header-anchor" href="#diwne-dear-imgui-wrapper-node-editor" aria-label="Permalink to &quot;DIWNE - Dear Imgui Wrapper Node Editor&quot;">​</a></h1><p>Custom implementation of node editor for Dear ImGui library.</p><h2 id="node-editor" tabindex="-1">Node editor <a class="header-anchor" href="#node-editor" aria-label="Permalink to &quot;Node editor&quot;">​</a></h2><p>Node editor is utility that provide organizing graphical view in graph style. Graph consist of nodes (vertices) that can be connected via links (edges). Content and graphical representation of nodes are up to you, as well as graphical representation of links.</p><p>Nodes are placed in infinity 2D space, and they are movable over this space within window, as well as you can move whole space. Nodes can overlap, as well as links.</p><h2 id="philosophy-of-diwne" tabindex="-1">Philosophy of DIWNE <a class="header-anchor" href="#philosophy-of-diwne" aria-label="Permalink to &quot;Philosophy of DIWNE&quot;">​</a></h2><p>DIWNE is &quot;node editor&quot; extension of Dear ImGui library. Base idea is not implement any additional function (such as rendering or input functions), but only wrap existing Dear ImGui functions. All DIWNE functions provide (just) right order of calling of Dear ImGui functions that leads to node editor behavior.</p><p>DIWNE is coded in OOP style and all functions can be overridden. This allows you unlimited customization of view and behavior.</p><p>DIWNE does not provide structure where are nodes and links stored. You can store them wherever you want as well as it is up to you call &quot;draw&quot; function on those you want to be drawn.</p><h2 id="base-architecture-of-diwne" tabindex="-1">Base architecture of DIWNE <a class="header-anchor" href="#base-architecture-of-diwne" aria-label="Permalink to &quot;Base architecture of DIWNE&quot;">​</a></h2><p>DIWNE is coded in OOP style and <a href="https://en.wikipedia.org/wiki/Template_method_pattern" target="_blank" rel="noreferrer">Template method pattern</a>. That means, that DIWNE function implement right order of calling functions, and you will fill in (override) function just for (for example) content of node.</p><p>All objects have access to &quot;Diwne (see below)&quot; object, so they can share information via it if needed. Your objects for node, link, editor etc. will be inherited from DIWNE objects, and so they can add any attributes or functionality.</p><p>Simplified procedure is override <code>content()</code> function of object and call <code>drawDiwne()</code> function in place in code where you want to draw it. Functions with &quot;Diwne&quot; in name (<code>drawDiwne()</code>, <code>xxxDiwne()</code> etc.) are not intended to override (but you can do it). Be aware what you are doing while overriding those functions.</p><h3 id="oop-structure" tabindex="-1">OOP structure <a class="header-anchor" href="#oop-structure" aria-label="Permalink to &quot;OOP structure&quot;">​</a></h3><ul><li><p><a href="./../cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html"><code>DiwneObject</code></a> (see <code>DiwneObject.h</code> file)</p><p>Class from with all DIWNE objects inherit. Provide base atributes:</p><ul><li><code>diwne</code>: reference to &quot;node-editor object&quot; that represents whole node-editor.</li></ul><p>Provides sharing inter-object information:</p><ul><li><p><code>m_idDiwne</code>, <code>m_labelDiwne</code>, <code>m_popupLabelDiwne</code>: identification attributes used for creating ImGui id.</p></li><li><p><code>m_inner_interaction_happen</code>, <code>m_inner_interaction_happen_previous_draw</code>: interaction information.</p></li><li><p><code>m_drawMode</code>, <code>m_selectable</code>, <code>m_isHeld</code>, <code>m_isDragged</code>, etc.: actual usage information.</p></li></ul></li><li><p><a href="./../cpp-api-reference/classes/classDIWNE_1_1Diwne.html"><code>Diwne</code></a> (see <code>diwne.h</code> file)</p><p>Class that represents whole node-editor. Process &quot;workarea&quot; aka &quot;2D space&quot; operation.</p><p>Your main (node-editor) object will inherit from this and then override content() function with all content you want to see. When you want draw node-editor, call drawDiwne() on this object.</p><p>Provide shared functions and information and workarea information:</p><ul><li><code>mp_settingsDiwne</code>: store information with setting (colors, margins, sizes etc.).</li><li><code>m_diwneAction</code>, <code>m_diwneAction_previousFrame</code>: actual and previous user action.</li><li><code>mp_lastActivePin</code>, <code>mp_lastActiveNode</code>: last active objects used by user.</li><li><code>m_workAreaScreen</code>, <code>m_workAreaDiwne</code>: what part of workarea is visible on screen. m_workAreaScreen is basically. just window, <code>m_workAreaDiwne</code> depends on user translation of space (<code>m_workAreaDiwne.Min</code>), window size and zoom. See <code>Diwne::updateWorkAreaRectangles()</code>.</li><li>transform coordinates between ImGui-screen-workarea.</li><li>draw some basic icons.</li><li><code>bypass...()</code> functions: name &quot;bypass&quot; is confusing a little bit. Wrap original ImGui functions which allows you to change default ImGui behavior by override bypass...() function.</li></ul></li><li><p><a href="./../cpp-api-reference/classes/classDIWNE_1_1Node.html"><code>Node</code></a> (see <code>Node.h</code>)</p><p>Object representing node (vertex). Your Node will inherit from this object. For draw node call <code>drawDiwne()</code> on object</p><p>Node has graphical parts:</p><div class="language- vp-adaptive-theme"><button title="Copy Code" class="copy"></button><span class="lang"></span><pre class="shiki github-dark vp-code-dark"><code><span class="line"><span style="color:#e1e4e8;">-------------------</span></span>
<span class="line"><span style="color:#e1e4e8;">|      Top        |</span></span>
<span class="line"><span style="color:#e1e4e8;">-------------------</span></span>
<span class="line"><span style="color:#e1e4e8;">|Left|Middle|Right|    -&gt; |Left|Middle|Right| == Center</span></span>
<span class="line"><span style="color:#e1e4e8;">-------------------</span></span>
<span class="line"><span style="color:#e1e4e8;">|     Bottom      |</span></span>
<span class="line"><span style="color:#e1e4e8;">-------------------</span></span></code></pre><pre class="shiki github-light vp-code-light"><code><span class="line"><span style="color:#24292e;">-------------------</span></span>
<span class="line"><span style="color:#24292e;">|      Top        |</span></span>
<span class="line"><span style="color:#24292e;">-------------------</span></span>
<span class="line"><span style="color:#24292e;">|Left|Middle|Right|    -&gt; |Left|Middle|Right| == Center</span></span>
<span class="line"><span style="color:#24292e;">-------------------</span></span>
<span class="line"><span style="color:#24292e;">|     Bottom      |</span></span>
<span class="line"><span style="color:#24292e;">-------------------</span></span></code></pre></div><p>For change content of <code>Node</code> override <code>topContent()</code>, <code>leftContent()</code> etc. functions.</p><p><code>content()</code> function is already prepared (see <code>Node::content()</code>) so you need not override it (if you do not want).</p><p>Size of node and its parts are updated in the end of node drawing, see <code>Node::updateSizes()</code>.</p></li><li><p><a href="./../cpp-api-reference/classes/classDIWNE_1_1Pin.html"><code>Pin</code></a> (see <code>Pin.h</code>)</p><p>Object representing point (usually square) from and to which link is connected. Pin will be probably part of your node (can be invisible) and link will lead to it.</p><p>Pull the link from pin is possible by mouse and pin is only possible place from with it is possible (by default).</p><p>Main atributes:</p><ul><li><code>m_connectionPointDiwne</code>: point to/from which will be link drawn (by default).</li></ul></li><li><p><a href="./../cpp-api-reference/classes/classDIWNE_1_1Link.html"><code>Link</code></a> (see <code>Link.h</code> file)</p><p>Object representing link (edge in graph). It is Bézier curve with four parameters - end points and one control point for each end point.</p></li></ul><h2 id="todo" tabindex="-1">TODO <a class="header-anchor" href="#todo" aria-label="Permalink to &quot;TODO&quot;">​</a></h2><ul><li>Minimal working example</li><li>Re-name bypass functions</li></ul>`,17),s=[t];function r(d,c,l,p,u,h){return o(),i("div",null,s)}const m=e(a,[["render",r]]);export{w as __pageData,m as default};
