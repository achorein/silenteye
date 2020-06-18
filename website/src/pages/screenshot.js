import React from "react"
import Layout from "../components/Layout"
import { withPrefix } from "gatsby"

export default function Home() {
  return (
    <Layout>
        <h3 style={{marginTop: 0}}>Screenshots</h3>

        <h4 style={{marginBottom: 0}}>Load an image into <i>SilentEye</i></h4>
        <img src={withPrefix('/images/app/image.png')} alt="Encode information into image" title="Hide into images" />

        <h4 style={{marginBottom: 0}}>Encode a message</h4>
        <img alt="Encode dialog" src={withPrefix('/images/app/encode.png')}/>

        <h4 style={{marginBottom: 0}}>Decode a message</h4>
        <img alt="Decode dialog" src={withPrefix('/images/app/decode.png')}/>
    </Layout>
  )
}