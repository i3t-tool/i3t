import { defineConfig } from 'vitepress'
import { generateSidebar } from "vitepress-sidebar";

const autogenerate = (text: string, link: string) => {
  const base = `cpp-api-reference/${link}`
  const result = generateSidebar({
    documentRootPath: 'Docs',
    scanStartPath: base,
    useTitleFromFrontmatter: true,
    rootGroupLink: `/${base}/`,
    rootGroupText: text,
    rootGroupCollapsed: true,
  })

  for (const item of result) {
    for (const subitem of item.items) {
      subitem.link = base + '/' + subitem.link
    }
  }

  return result
}

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "I3T",
  description: "I3T",
  // Disable markdown-it-anchor which breaks markdown interpolation
  // ```
  // # Your title {#your-custom-id}
  // ```
  markdown: { attrs: { disable: true } },
  ignoreDeadLinks: true,
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Download', link: 'https://github.com/i3t-tool/i3t/releases/latest' },
      // { text: 'Examples', link: '/markdown-examples' }
    ],

    sidebar: [
      {
        text: 'Getting Started',
        items: [
          { text: 'Introduction', link: '/introduction' },
          { text: 'Manual', link: '/manual' },
          { text: 'Making own tutorials', link: '/tutorials' },
        ]
      },
      {
        text: 'Developer Guide',
        link: '/developer-guide/',
        items: [
          { text: 'Build', link: '/developer-guide/build' },
          { text: 'Architecture', link: '/developer-guide/architecture' },
          { text: 'Code Style', link: '/developer-guide/code-style' },
          { text: 'DIWNE library', link: '/developer-guide/diwne' },
          { text: 'DIWNE in I3T', link: '/developer-guide/diwne-in-i3t' },
          { text: 'How to add new node', link: '/developer-guide/how-to-add-new-node' },
        ]
      },
      {
        text: 'Lua API Reference',
        collapsed: true,
        items: [
          { text: 'Common', link: '/lua-api-reference/common' },
          { text: 'Tutorial', link: '/lua-api-reference/tutorial' },
          { text: 'Workspace', link: '/lua-api-reference/workspace' },
        ]
      },
      {
        text: 'C++ API Reference',
        collapsed: true,
        items: [
          ...autogenerate('Classes', 'classes'),
          ...autogenerate('Modules', 'modules'),
          ...autogenerate('Namespaces', 'namespaces'),
        ]
      }
    ],

    socialLinks: [
      { icon: 'github', link: 'https://github.com/i3t-tool/i3t' }
    ],
  }
})
