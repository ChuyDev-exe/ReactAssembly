import { describe, it, expect } from 'vitest'
import { render, screen } from '@testing-library/react'
import App from '../App'

describe('App Component', () => {
    it('renders without crashing', () => {
        render(<App />)
        expect(document.querySelector('.App')).toBeTruthy()
    })

    it('displays the main heading', () => {
        render(<App />)
        const heading = screen.getByText(/WebAssembly/i)
        expect(heading).toBeInTheDocument()
    })

    it('contains canvas element for WebAssembly rendering', () => {
        render(<App />)
        const canvas = document.querySelector('#canvas')
        expect(canvas).toBeInTheDocument()
    })
})
