import { defineConfig } from 'vitepress'
// import { generateSidebar } from "vitepress-sidebar";


// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "I3T",
  description: "I3T",
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
      }
    ],

    socialLinks: [
      { icon: 'github', link: 'https://github.com/i3t-tool/i3t' }
    ],
  },
  // TODO Remove me after setting custom domain
  base: '/i3t/',
})
