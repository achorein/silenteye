import React from "react"
import Layout from "../components/Layout"
import { withPrefix } from "gatsby"

export default function Home() {
  return (
    <Layout>
<h3 style={{marginTop: 0}}>Roadmap</h3>

<img src={withPrefix('/images/icon/about.png')} alt="roadmap" class="image" style={{margin: '0pt 10px 0pt 0pt', float: 'left'}} />
<p>
	Feel free to report bugs or ask for new feactures on our <a href="https://github.com/achorein/silenteye/issues">project tracker</a>
</p>
<p>
	Roadmap on github : <a href="https://github.com/achorein/silenteye/milestones">https://github.com/achorein/silenteye/milestones</a>
</p>
    </Layout>
	)
  }