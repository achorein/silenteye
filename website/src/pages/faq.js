import React from "react"
import Layout from "../components/Layout"

export default function Home() {
  return (
    <Layout>
<h3 style={{marginTop: 0}}>FAQ (Frequently Ask Question)</h3>

<p>
	<ul>
		<li>
			<b>1. Is this software for educational purpose or for business? Are you thinking about commercial license and making money?</b><br/>
			<p>
				SilentEye is a 100% Free software, we have no interest in making money and will not. SilentEye is an open source project, 
				that means you have access to the end product's source materials. Anyone may join and contribute, the direction and goals 
				are determined collaboratively by all members of the community, and the resulting work is made freely available.
			</p>
		</li>
		<li>
			<b>2. Why does SilentEye use Qt?</b>
			<p>
				The most important reason is to have a platform abstraction for most Unix and Windows. 
				Another reason is for the nice and bug free utility classes. And the last reason is the beautiful GUI provided by the Qt platform!
			</p>
		</li>
	</ul>
</p>
</Layout>
  )
}